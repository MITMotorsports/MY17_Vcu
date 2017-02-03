/**
 * We want to have a logging process with a periodicity independent of the actual control loop.
 * My idea is to implement this as a 10ms period event that logs certain things in a multiple of 10ms
 * (i.e. battery events might be logged with a 100ms resolution but control events could be 20ms and * faults could be 1000ms).
 *
 * Actually sequence flow should go

  Actually sequence flow should go as follows:

  One main loop running as fast as possible.

  This loop first checks for any available CAN messages and processes them.
    It does so by outputting a sequence of Commands, where a command could be:
      PinCommand (turn some pins on or off)
      CanCommand (send some CAN messages)
      LogCommand (log something with some associated timestamp)
      etc
  The loop then takes this set of output commands and processes them:
    PinCommands have pins flipped on/off
    CanCommands have messages sent
    LogCommands have print commands called
      Note that these are non-blocking so paradoxically we actually should print very often
      rather than batch our printing - arduino does free background resolution as long as the 
      buffer isn't too large.

  There might potentially be auxilary loops (e.g. something that checks state of buttons every 1s or so)
  but it's essential that these loops are short in duration as they are essentially blocking execution of the primary loop.

  Note that this system assumes that we want to send CAN messages only in response to received input events
    If this assumption changes, we should have an additional step in the main loop that checks if a given time has 
    elapsed - don't try to do some timing management using softtimer or anything because that could block the main thread.

  It's an open question of whether the CAN buffer will tolerate being potentially read from on such an infrequent basis.
    Perhaps we might need to fully empty the CAN read buffer at the start of the loop and then process each message in sequence
    This is something to experiment with.
 */
