//
// Created by Alon Sportes on 23/12/2023.
//

// #ifndef SETTINGS_H
// #define SETTINGS_H
#ifdef SETTINGS_H

    #include "CustomPlotsDirectory.cpp"

/**
 * @class Settings
 * @brief A class that inherits from CustomPlotsDirectory and provides additional settings for the analysis.
 * @details The Settings class inherits from the CustomPlotsDirectory class, which provides functionality for configuring custom plot directories based on various cut configurations. The
 * Settings class can be used to define additional settings for the analysis, such as auto-disabling certain variables based on specific conditions. The AutoDisableVariables function can be
 * implemented to automatically disable certain variables based on predefined criteria, allowing for more streamlined and efficient analysis. By using inheritance, the Settings class can
 * leverage the functionality of the CustomPlotsDirectory class while also providing additional features specific to the analysis being performed.
 * @note The implementation of the AutoDisableVariables function will depend on the specific criteria for disabling variables, and should be designed to ensure that it does not inadvertently
 * disable important variables that are necessary for the analysis. It is also important to ensure that any settings defined in the Settings class are properly documented and communicated to
 * other developers working on the project to avoid confusion and ensure consistency in the analysis approach.
 */
class Settings : public CustomPlotsDirectory {
   private:
   public:
    // Default constructor ---------------------------------------------------------------------------------------------------------------------------------------------

    Settings() = default;

    // AutoDisableVariables constructor --------------------------------------------------------------------------------------------------------------------------------

    void AutoDisableVariables();
};

#endif  // SETTINGS_H
