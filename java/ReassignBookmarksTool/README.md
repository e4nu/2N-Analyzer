
# ReassignBookmarksTool

**Author:** Alon Sportes  
**Project:** `2N-Analyzer-e4nu`

# Overview

A small Java 11 command line utility that manipulates PDF bookmarks (outline entries) using Apache PDFBox and Jackson.

It supports three workflows:

1. **extract**: read bookmarks from an input PDF and write them to a JSON file
2. **reassign**: remove existing bookmarks from a PDF and add new ones defined in a JSON file
3. **strip**: remove all bookmarks from a PDF (optionally flattening the file to images, removing the text layer)

This tool is useful when you need to preserve, clean, or rebuild a PDF outline tree after merging PDFs, converting to images, or generating PDFs from external tools.

## What is in this folder

- `ReassignBookmarksTool.java`  
  Source code for the tool.

- `build_ReassignBookmarksTool.sh`  
  Build script that compiles the source with Java 11 and produces `ReassignBookmarksTool.jar`.

- `ReassignBookmarksTool.jar` (generated)  
  Runnable JAR produced by the build script.

- `lib/` (expected)  
  External JAR dependencies (PDFBox, Jackson, etc.). The build script assumes they exist in this folder.

## Features and behavior

### Bookmark model

Bookmarks are represented by the `BookmarkEntry` class:

- `title` (string): visible bookmark name
- `page` (int, 1-based): destination page number
- `parent` (string, optional): parent title, primarily for bookkeeping
- `children` (array): nested children for hierarchical outlines

### Flat vs hierarchical outlines

The tool can work with:

- **Flat lists** of bookmarks
- **Hierarchical** bookmark trees

Hierarchy can be derived in two ways:

1. Using a title path separator: `A > B > C` in the bookmark title
2. Passing the `hierarchical` flag to `reassign` (to force conversion)

Internally, `convertToHierarchy(...)` splits titles on the `>` character, trims each segment, and builds a nested structure while keeping insertion order.

### Page safety

When creating outline items, the page index is clamped to `[0, numberOfPages - 1]` to avoid out-of-range crashes.

### Stripping and flattening

`strip` always removes the PDF outline tree.

If `preserveText` is not provided, the tool renders each page to a 300 DPI image and creates a new PDF composed of those images. This removes selectable text and other searchable layers.

## Usage

Run either with the generated runnable JAR, or directly with the classpath.

### Common examples

Extract bookmarks from an existing PDF:

```bash
java -jar ReassignBookmarksTool.jar extract input.pdf
```

This writes a `bookmarks.json` file next to `input.pdf`.

Strip bookmarks only (keep text layer):

```bash
java -jar ReassignBookmarksTool.jar strip input.pdf output.pdf preserveText
```

Strip bookmarks and flatten pages to images (removes text layer):

```bash
java -jar ReassignBookmarksTool.jar strip input.pdf output.pdf
```

Reassign bookmarks from JSON (flat list):

```bash
java -jar ReassignBookmarksTool.jar reassign input.pdf bookmarks.json output.pdf
```

Reassign bookmarks from JSON and force hierarchical conversion:

```bash
java -jar ReassignBookmarksTool.jar reassign input.pdf bookmarks.json output.pdf hierarchical
```

### Notes on arguments

- `extract` expects: `extract input.pdf`  
  Output is always written as `bookmarks.json` in the same directory as `input.pdf`.

- `strip` expects: `strip input.pdf output.pdf [preserveText]`

- `reassign` expects: `reassign input.pdf bookmarks.json output.pdf [hierarchical]`

## JSON format

The JSON is an array of entries. For hierarchical outlines, entries may include a `children` array.

Minimal flat entry:

```json
[
  {"title": "Introduction", "page": 1},
  {"title": "Results", "page": 10}
]
```

Hierarchical entry (nested):

```json
[
  {
    "title": "Chapter 1",
    "page": 1,
    "children": [
      {"title": "Section 1.1", "page": 2},
      {"title": "Section 1.2", "page": 5}
    ]
  }
]
```

Path-based hierarchy (flat list using `>` in titles) is also supported. For example:

```json
[
  {"title": "Chapter 1 > Section 1.1", "page": 2},
  {"title": "Chapter 1 > Section 1.2", "page": 5}
]
```

When hierarchy conversion runs, intermediate nodes like `Chapter 1` are created automatically.

## Building

### Prerequisites

- Java 11 (or newer, but compilation targets Java 11)
- A `lib/` directory containing the required dependency JARs

The code imports:

- Apache PDFBox (`org.apache.pdfbox.*`)
- Jackson Databind (`com.fasterxml.jackson.databind.*`)

### Build using the provided script

From this directory:

```bash
./build_ReassignBookmarksTool.sh
```

This compiles `ReassignBookmarksTool.java` with:

- `javac --release 11 -cp ".:lib/*" ReassignBookmarksTool.java`

And produces:

- `ReassignBookmarksTool.class`
- `ReassignBookmarksTool.jar`

### Running with an explicit classpath

If you do not want to use the runnable JAR:

```bash
java -cp .:lib/* ReassignBookmarksTool extract input.pdf
java -cp .:lib/* ReassignBookmarksTool reassign input.pdf bookmarks.json output.pdf
```

## Implementation overview

- `extractBookmarks(...)` loads the PDF, traverses its outline tree, and writes bookmarks to JSON.
- `extractOutlineItems(...)` walks the outline tree recursively, skipping titles that start with `Page `.
- `stripBookmarks(...)` removes the outline tree and optionally rebuilds the PDF by rendering pages to images.
- `reassignBookmarks(...)` clears the outline tree, reads bookmarks from JSON, optionally converts to hierarchy, and writes a new outline.
- `createOutlineItem(...)` builds a `PDOutlineItem` for each entry and recurses over children.

## Caveats

- The current `extract` mode always converts extracted bookmarks to a hierarchy before writing JSON.
- `reassign` deletes the JSON file it used and may delete the input PDF if it is treated as a temporary file and is not the same path as the output. Use `reassign` on copies if you want to keep originals.

## Troubleshooting

- **No bookmarks found**: the input PDF has no outline tree.
- **ClassNotFoundException / NoClassDefFoundError**: ensure all dependency JARs are in `lib/` and you are using `-cp .:lib/*`.
- **Pages jump to the wrong location**: verify that JSON page numbers are 1-based.