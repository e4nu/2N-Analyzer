//
// Created by Alon Sportes on 23/12/2023.
//

// #ifndef SETTINGS_H
// #define SETTINGS_H
#ifdef SETTINGS_H

    #include "CustomPlotsDirectory.cpp"

class Settings : public CustomPlotsDirectory {
   private:
   public:
    // Default constructor ---------------------------------------------------------------------------------------------------------------------------------------------

    Settings() = default;

    // AutoDisableVariables constructor --------------------------------------------------------------------------------------------------------------------------------

    void AutoDisableVariables();
};

#endif  // SETTINGS_H
