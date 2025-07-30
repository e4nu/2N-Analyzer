// package HipoLooper.framework.java.ReassignBookmarksTool;
import com.fasterxml.jackson.databind.*;
import java.io.*;
import java.util.*;
import org.apache.pdfbox.pdmodel.PDDocument;
import org.apache.pdfbox.pdmodel.PDPage;
import org.apache.pdfbox.pdmodel.common.PDPageLabels;
import org.apache.pdfbox.pdmodel.interactive.documentnavigation.outline.*;

public class ReassignBookmarksTool {
    // ANSI color codes for output
    public static final String RESET = "\u001B[0m";
    public static final String RED = "\u001B[31m"; // Errors
    public static final String GREEN = "\u001B[32m"; // Success/info
    public static final String CYAN = "\u001B[36m"; // Warnings (changed to cyan)

    private static void printHierarchy(List<BookmarkEntry> entries, String indent) {
        for (BookmarkEntry e : entries) {
            System.out.println(indent + "- " + e.title + " (Page " + e.page + ")");
            if (e.children != null) { printHierarchy(e.children, indent + "  "); }
        }
    }

    /**
     * Converts a flat list of bookmarks with '>' in the title into a nested hierarchy.
     */
    private static List<BookmarkEntry> convertToHierarchy(List<BookmarkEntry> flatBookmarks) {
        Map<String, BookmarkEntry> allBookmarks = new LinkedHashMap<>();
        List<BookmarkEntry> roots = new ArrayList<>();

        for (BookmarkEntry entry : flatBookmarks) {
            System.out.println("Processing entry: " + entry.title + " (Page " + entry.page + ")");
            String[] parts = entry.title.split(">");
            StringBuilder fullPath = new StringBuilder();
            BookmarkEntry parent = null;

            for (int i = 0; i < parts.length; i++) {
                String part = parts[i].trim();
                if (part.isEmpty()) continue;

                if (fullPath.length() > 0) fullPath.append(" > ");
                fullPath.append(part);

                // Debug: print level and key
                System.out.println("  Level " + i + ": " + parts[i] + " -> Key: " + fullPath.toString());

                String key = fullPath.toString();
                BookmarkEntry current = allBookmarks.get(key);
                if (current == null) {
                    System.out.println("  Creating new entry for: " + key);
                } else {
                    System.out.println("  Found existing entry for: " + key);
                }

                if (current == null) {
                    current = new BookmarkEntry();
                    current.title = part;
                    if (i == parts.length - 1) { current.page = entry.page; }
                    current.children = new ArrayList<>();
                    allBookmarks.put(key, current);

                    if (parent != null) {
                        parent.children.add(current);
                        System.out.println("  - Added child '" + current.title + "' to parent '" + parent.title + "'");
                    } else {
                        roots.add(current);
                        System.out.println("- Added root bookmark: '" + current.title + "'");
                    }
                }

                parent = current;
            }
        }

        System.out.println("Finished building hierarchy. Total root bookmarks: " + roots.size());

        printHierarchy(roots, "");

        return roots;
    }

    static class BookmarkEntry {
        public String title;
        public int page;
        public String parent; // optional
        public List<BookmarkEntry> children = new ArrayList<>();

        // Needed for Jackson deserialization
        public BookmarkEntry() {}
        public BookmarkEntry(String title, int page) {
            this.title = title;
            this.page = page;
        }

        public String getTitle() {
            return title;
        }

        public int getPage() {
            return page;
        }

        public List<BookmarkEntry> getChildren() {
            return children;
        }
    }

    /**
     * Entry point for the ReassignBookmarksTool utility.
     *
     * This method parses command-line arguments to determine the operation mode.
     * Supported operations:
     * - "extract": Extracts bookmarks from a PDF and writes them to a JSON file.
     * - "reassign": Reassigns bookmarks from a JSON file to a PDF, optionally in hierarchical format.
     *
     * Arguments:
     *   extract input.pdf output.json
     *   reassign input.pdf bookmarks.json output.pdf [hierarchical]
     *
     * @param args Command-line arguments specifying the operation and its parameters.
     * @throws Exception If an error occurs during file I/O or processing.
     */
    public static void main(String[] args) throws Exception {
        System.out.println(GREEN + "\nArguments received:" + RESET);

        for (int i = 0; i < args.length; i++) { System.out.println(GREEN + "  arg[" + i + "]: " + RESET + args[i]); }

        if (args.length < 2) {
            System.out.println(CYAN + "Usage:" + RESET);
            System.out.println(GREEN + "  java -jar ReassignBookmarksTool.jar input.pdf bookmarks.json output.pdf" + RESET);
            System.out.println(GREEN + "  java -jar ReassignBookmarksTool.jar extract input.pdf output.json" + RESET);
            System.out.println(GREEN + "  java -jar ReassignBookmarksTool.jar reassign input.pdf bookmarks.json output.pdf [hierarchical]" + RESET);
            return;
        }

        if (args[0].equals("extract")) {
            String inputPDF = args[1];
            File inputFile = new File(inputPDF);
            String outputJSON = new File(inputFile.getParent(), "bookmarks.json").getAbsolutePath();
            extractBookmarks(inputPDF, outputJSON);

            System.out.println(GREEN + "\nBookmark extraction completed!" + RESET);
            System.out.println(GREEN + "Extracted bookmarks saved to: " + RESET + outputJSON + "\n");
        } else if (args[0].equals("strip")) {
            if (args.length < 3 || args.length > 4) {
                System.out.println(RED + "Usage: strip <input.pdf> <output.pdf> [preserveText]" + RESET);
                return;
            }
            boolean preserveText = args.length == 4 && args[3].equalsIgnoreCase("preserveText");
            stripBookmarks(args[1], args[2], preserveText);

            System.out.println(GREEN + "\nBookmark strip completed!" + RESET);
            System.out.println(GREEN + "Stripped PDF file saved to: " + RESET + args[2] + "\n");
        } else if (args[0].equals("reassign")) {
            boolean hierarchical = args.length > 4 && args[4].equalsIgnoreCase("hierarchical");
            String inputPDF = args[1];
            String bookmarkJSON = args[2];
            String outputPDF = args[3];
            reassignBookmarks(inputPDF, bookmarkJSON, outputPDF, hierarchical);

            System.out.println(GREEN + "\nBookmark reassignment completed!" + RESET);
            System.out.println(GREEN + "Reassign PDF file saved to: " + RESET + outputPDF + "\n");
            return;
        }
    }

    /**
     * Extracts bookmarks from a PDF file and writes them to a JSON file.
     *
     * This method loads the input PDF file, retrieves its outline/bookmarks (if any),
     * and stores a structured representation of those bookmarks in a JSON file.
     * Bookmarks with titles starting with "Page " are ignored.
     *
     * @param inputPDF     Path to the input PDF file from which bookmarks will be extracted.
     * @param outputJSON   Path to the output JSON file where the extracted bookmarks will be saved.
     * @throws IOException If reading the PDF or writing the JSON file fails.
     */
    public static void extractBookmarks(String inputPDF, String outputJSON) throws IOException {
        try (PDDocument document = PDDocument.load(new File(inputPDF))) {
            PDDocumentOutline outline = document.getDocumentCatalog().getDocumentOutline();
            if (outline == null) {
                System.out.println(RED + "No bookmarks found." + RESET);
                return;
            }

            List<BookmarkEntry> bookmarks = new ArrayList<>();
            extractOutlineItems(outline, null, bookmarks, document);

            // Convert bookmarks to hierarchy before writing
            bookmarks = convertToHierarchy(bookmarks);

            ObjectMapper mapper = new ObjectMapper();
            mapper.writerWithDefaultPrettyPrinter().writeValue(new File(outputJSON), bookmarks);
        }
    }

    /**
     * Recursively extracts bookmark entries from a PDF outline node.
     *
     * This helper method walks through the outline tree structure of a PDF,
     * filters out any bookmarks whose titles start with "Page ",
     * and populates a list with bookmark data including hierarchy if applicable.
     *
     * @param node        The current PDF outline node.
     * @param parentTitle The title of the parent bookmark, or null if at root level.
     * @param list        The list to populate with extracted BookmarkEntry instances.
     * @param doc         The PDF document being processed.
     * @throws IOException If accessing the PDF structure fails.
     */
    private static void extractOutlineItems(PDOutlineNode node, String parentTitle, List<BookmarkEntry> list, PDDocument doc) throws IOException {
        PDOutlineItem current = node.getFirstChild();
        while (current != null) {
            String title = current.getTitle();
            if (!title.startsWith("Page ")) {
                BookmarkEntry entry = new BookmarkEntry();
                entry.title = title;
                entry.page = doc.getPages().indexOf(current.findDestinationPage(doc)) + 1;
                if (parentTitle != null) entry.parent = parentTitle;
                list.add(entry);

                if (current.hasChildren()) { extractOutlineItems(current, title, list, doc); }
            }
            current = current.getNextSibling();
        }
    }

    /**
     * Strip all bookmarks from a PDF and save a new file with the same content but no outline tree.
     */
    public static void stripBookmarks(String inputPDF, String outputPDF, boolean preserveText) throws IOException {
        try (PDDocument document = PDDocument.load(new File(inputPDF))) {
            document.getDocumentCatalog().setDocumentOutline(null);

            if (!preserveText) {
                // Remove text content from all pages (e.g., flatten to images)
                org.apache.pdfbox.rendering.PDFRenderer pdfRenderer = new org.apache.pdfbox.rendering.PDFRenderer(document);
                PDDocument imageDoc = new PDDocument();
                for (int page = 0; page < document.getNumberOfPages(); ++page) {
                    java.awt.image.BufferedImage bim = pdfRenderer.renderImageWithDPI(page, 300);
                    org.apache.pdfbox.pdmodel.PDPage newPage = new org.apache.pdfbox.pdmodel.PDPage(document.getPage(page).getMediaBox());
                    imageDoc.addPage(newPage);
                    org.apache.pdfbox.pdmodel.PDPageContentStream contentStream = new org.apache.pdfbox.pdmodel.PDPageContentStream(imageDoc, newPage);
                    org.apache.pdfbox.pdmodel.graphics.image.PDImageXObject pdImage = org.apache.pdfbox.pdmodel.graphics.image.LosslessFactory.createFromImage(imageDoc, bim);
                    contentStream.drawImage(pdImage, 0, 0, newPage.getMediaBox().getWidth(), newPage.getMediaBox().getHeight());
                    contentStream.close();
                }

                imageDoc.save(outputPDF);
                imageDoc.close();
                System.out.println(GREEN + "Bookmarks stripped and content flattened (no text layer): " + RESET + outputPDF);
                return;
            }

            document.save(new File(outputPDF));
            System.out.println(GREEN + "Bookmarks stripped and PDF saved: " + RESET + outputPDF);
        } catch (IOException e) {
            System.out.println(RED + "Error stripping bookmarks from PDF: " + RESET + e.getMessage());
            throw e;
        }
    }

    /**
     * Reassigns bookmarks to a PDF file based on a JSON definition.
     *
     * This method removes any existing bookmarks from the input PDF, then
     * adds new bookmarks as defined in a JSON file. Handles nested bookmarks using " > " in titles.
     *
     * @param inputPDF     Path to the PDF file to modify.
     * @param bookmarkJSON Path to the JSON file containing bookmark definitions.
     * @param outputPDF    Path where the modified PDF will be saved.
     * @param hierarchical If true, organizes bookmarks hierarchically based on parent relationships.
     * @throws IOException If reading the PDF or JSON, or writing the new PDF fails.
     */
    public static void reassignBookmarks(String inputPDF, String bookmarkJSON, String outputPDF, boolean hierarchical) throws IOException {
        try (PDDocument document = PDDocument.load(new File(inputPDF))) {
            // Remove old bookmarks
            document.getDocumentCatalog().setDocumentOutline(null);

            // Read new bookmarks
            ObjectMapper mapper = new ObjectMapper();
            List<BookmarkEntry> entries = Arrays.asList(mapper.readValue(new File(bookmarkJSON), BookmarkEntry[].class));

            List<BookmarkEntry> finalEntries = hierarchical ? convertToHierarchy(entries) : entries;
            addBookmarksToOutline(document, finalEntries);

            document.save(outputPDF);

            // Delete temporary files if they exist
            File jsonFile = new File(bookmarkJSON);
            File strippedFile = new File(inputPDF);
            if (jsonFile.exists()) {
                if (jsonFile.delete()) {
                    System.out.println(GREEN + "Temporary bookmark file deleted: " + RESET + bookmarkJSON);
                } else {
                    System.out.println(RED + "Failed to delete temporary bookmark file: " + RESET + bookmarkJSON);
                }
            }
            if (strippedFile.exists() && !strippedFile.getAbsolutePath().equals(outputPDF)) {
                if (strippedFile.delete()) {
                    System.out.println(GREEN + "Temporary stripped PDF deleted: " + RESET + inputPDF);
                } else {
                    System.out.println(RED + "Failed to delete temporary stripped PDF: " + RESET + inputPDF);
                }
            }
        }
    }

    /**
     * Adds bookmarks to the PDF outline, handling recursive child bookmarks.
     */
    private static void addBookmarksToOutline(PDDocument document, List<BookmarkEntry> bookmarks) {
        PDDocumentOutline outline = new PDDocumentOutline();
        document.getDocumentCatalog().setDocumentOutline(outline);

        System.out.println(GREEN + "\nAdding bookmarks to outline...\n" + RESET);

        for (BookmarkEntry entry : bookmarks) {
            PDOutlineItem item = createOutlineItem(entry, document);
            outline.addLast(item);
            System.out.println("- Added root bookmark: '" + (entry.getTitle() != null ? entry.getTitle() : "") + "'");
        }

        System.out.println("\n");

        outline.openNode();
    }

    /**
     * Recursively creates a PDOutlineItem from a BookmarkEntry and its children.
     * Page index is clamped to valid range to avoid IndexOutOfBoundsException.
     * All getTitle() calls are null-safe.
     */
    private static PDOutlineItem createOutlineItem(BookmarkEntry entry, PDDocument document) {
        // Clamp page index to valid range
        int pageIndex = Math.max(0, Math.min(document.getNumberOfPages() - 1, entry.getPage() - 1));
        PDPage page = document.getPage(pageIndex);
        PDOutlineItem item = new PDOutlineItem();
        String title = entry.getTitle() != null ? entry.getTitle().trim() : "";
        item.setTitle(title);
        item.setDestination(page);

        if (entry.getChildren() != null) {
            for (BookmarkEntry child : entry.getChildren()) {
                PDOutlineItem childItem = createOutlineItem(child, document);
                item.addLast(childItem);
                System.out.println("  ✔ Added child '" + (child.getTitle() != null ? child.getTitle() : "") + "' to parent '" + title + "'");
            }
        }

        return item;
    }
}