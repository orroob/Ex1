#pragma once

#ifndef HEADER_FILE
#include "HardCodedData.h"
/// <summary>
/// This function creates a process using winAPI's CreateProcessA function.
/// </summary>
/// <param name="command"> - a char* object containing the command to be executed.</param>
/// <param name="startinfo"> - a pointer to STARTUPINFO object that receives the startup_info data.</param>
/// <param name="procinfo"> - a pointer to PROCESS_INFORMATION object that receives the process_info data.</param>
/// <returns>Returns 0 if secceeded, 1 otherwise.</returns>
int openProcess(char* command, STARTUPINFO* startinfo, PROCESS_INFORMATION* procinfo);


/// <summary>
/// This function closes a handle to a process.
/// </summary>
/// <param name="hProcess"> - pointer to HANDLE holding the process to be closed.</param>
/// <returns>Returns 0 if secceeded, 1 otherwise.</returns>
int closeProcess(HANDLE* hProcess);

#endif
