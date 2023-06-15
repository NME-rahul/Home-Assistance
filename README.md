# Home-Assistance

### pseudo code:
    
    //open when needed
    var read_buffer;
    var write_buffer;
    var sense_count = 0;

    var error;

    Sense_carrier():
      //sense is SD card free or not?
      if free:
        send signal to device that it is reading/writing(busy signal);
        set sense_count as 0;
        set error as 0;
      if not free:
        sense_count = sense_count + 1;
        if sense_count < 15:
        sense again;
        set error message as "Error: SD card is busy with other device.";
      else:
        set led to HIGH to show count exceed;

    read_write_SDcard():
      Sense_carrier();
      if(!error):
        then read/write data as well as the buffer
        if write_buffer is available:
          then write;
        if read_buffer is available:
          then read;
      else:
        if write:
          add item in the write_buffer whenever the SD card is free it also writes the buffer
        if read:
          add(length, file_pointer) in read_buffer;


#### ESP

        //in loop program of ESP
        if inputmsg is available:
            if inputmsg != previous inputmsg:
                if inputmsg[0] = "some special commands":
                    //do operation on board
                if inputmsg[0] = "#":
                    read_write_SDcard(); //writing color info to lights in SD card
                else:
                    //device info
                    //device No, wakeup/sleep, 
                    read_write_SDcard(); //writing device info in SD card
                 
                 
                 
                 
#### Arduino
              
              // in loop program of Arduino
             if read_write_SDcard():
                //do operations on devices
            else:
                //set led as HIGH to show it is failed to read
