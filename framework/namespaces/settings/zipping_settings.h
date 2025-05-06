//
// Created by Alon Sportes on 24/04/2025.
//

#ifndef ZIPPING_SETTINGS_H
#define ZIPPING_SETTINGS_H

#include <iostream>

namespace zipping_settings {

// General zipping of a sample's output ---------------------------------------------------------------------------------------------------------------------------------

bool Save_Plots_folder_to_zip_files = true;
bool zipping_print_out = (Save_Plots_folder_to_zip_files && false);

// General zipping of all samples in the code run -----------------------------------------------------------------------------------------------------------------------

bool Collect_all_zip_files = (Save_Plots_folder_to_zip_files && false);
}  // namespace zipping_settings

#endif  // ZIPPING_SETTINGS_H
