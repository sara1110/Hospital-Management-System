#pragma once
#ifndef HOSPITALADMIN_H
#define HOSPITALADMIN_H

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "include\cmdui_utils.h"
#include "include\dataDefs.h"
#include "include\datautils.h"
#include "include\patientdb.h"
#include "include\doctordb.h"
#include "include\staffnursedb.h"
#include "include\appointmentdb.h"
#include "include\ehrdb.h"
#include "include\billdb.h"
#include "include\inventorydb.h"

// UI display utility
void hospAdminDisplayHospitalAdminMenu();
void hospAdminDisplayUserMngmntMenu();
void hospAdminDisplayPatientMngmntMenu();
void hospAdminDisplayDoctorMngmtMenu();
void hospAdminDisplayStaffNurseMngmtMenu();
void hospAdminDisplayInventoryMngmntMenu();

// Prints a horizontal bar as default separator
void printDefaultSeparator();

// Login (works as entry point function for 
// the whole hospital admin logic)
void hospitalAdminLogin(SystemState *sysState);

// State/UI functions 
// (indentation denotes hierarchy)
void hospitalAdminMenu(SystemState *sysState);
    void hospAdminUserManagementMenu(SystemState *sysState);
        void hospAdminPatientManagementMenu(SystemState *sysState);
            void hospAdminSeeAllPatients(SystemState *sysState);
            void hospAdminSearchPatient(SystemState *sysState);
            void hospAdminModifyPatient(SystemState *sysState);
            void hospAdminRegisterPatient(SystemState *sysState);
            void hospAdminUnregisterPatient(SystemState *sysState);
        void hospAdminDoctorManagementMenu(SystemState *sysState);
            void hospAdminSeeAllDoctors(SystemState *sysState);
            void hospAdminSearchDoctor(SystemState *sysState);
            void hospAdminModifyDoctor(SystemState *sysState);
            void hospAdminRegisterDoctor(SystemState *sysState);
            void hospAdminUnregisterDoctor(SystemState *sysState);
        void hospAdminStaffNurseManagementMenu(SystemState *sysState);
            void hospAdminSeeAllStaffNurses(SystemState *sysState);
            void hospAdminSearchStaffNurse(SystemState *sysState);
            void hospAdminModifyStaffNurse(SystemState *sysState);
            void hospAdminRegisterStaffNurse(SystemState *sysState);
            void hospAdminUnregisterStaffNurse(SystemState *sysState); 
    void hospAdminAppointmentMngmntMenu(SystemState *sysState);
        void hospAdminseeApointmentsAtSpfcDay(SystemState *sysState); 
        void hospAdminAddAppointment(SystemState *sysState);
        void hospAdminRescheduleAppointment(SystemState *sysState);
        void hospAdminSearchAppointment(SystemState *sysState);
        void hospAdminCancelAppointment(SystemState *sysState);
    void hospAdminBillMngmntMenu(SystemState *sysState);
        void hospAdminSeeBillsFromUser(SystemState *sysState); // search by user
        void hospAdminSearchBillAtSpecificDate(SystemState *sysState); // search by date
    void hospAdminEHRMngmntMenu(SystemState *sysState);
        void hospAdminSearchEHRFromPatient(SystemState *sysState); // search
        void hospAdminSetPatientEHR(SystemState *sysState); // set
            void hospAdminAddEHRHistory(SystemState *sysState);
            void hospAdminAddEHRPrescription(SystemState *sysState);
            void hospAdminAddEHRAllergy(SystemState *sysState);
    void hospAdminInventoryMngmntMenu(SystemState *sysState); 
        void hospAdminDisplayInventory(SystemState *sysState); 
        void hospAdminSearchInInventory(SystemState *sysState); 
        void hospAdminRegisterInvItemType(SystemState *sysState);
        void hospAdminAddQuantityToInventory(SystemState *sysState);
        void hospAdminUnregisterInvItemType(SystemState *sysState);
        void hospAdminRemoveQuantityFromInventory(SystemState *sysState);

#endif // HOSPITALADMIN_H
