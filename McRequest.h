#ifndef MC_REQUEST_H
#define MC_REQUEST_H

#include "Types.h"

#include "MY17_Can_Library.h"

void McRequest_update_mcRequest(Input_T *input, State_T *state, Output_T *output);

Can_MC_RegID_T McRequest_translate_mc_request(MC_Request_Type requestType);

#endif //MC_REQUEST_H
