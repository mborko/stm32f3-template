# Template project for the STM32F3Discovery Board

This is a template repository for getting the main peripherals working on a [STM32F3Discovery](http://www.st.com/web/en/catalog/tools/FM116/SC959/SS1532/PF254044) board.

## Toolchain

To get all dependencies and be ready to flash the STM32F3 Board, install the following packages:

        apt install cmake libusb-dev libusb-1.0.0-dev build-essential autoconf\
         cutecom git binutils-arm-none-eabi gcc-arm-none-eabi

## Flash-Tool

The newest Flash-Tool for the STM-Boards can be found at [STLINK](https://github.com/texane/stlink). Clone it and make a debian package, which then can be installed via dpkg:

        git clone https://github.com/texane/stlink  
        cd stlink  
        make clean  
        make package  
        sudo dpkg -i build/Release/stlink-1.4.0-12-g95b6e03-amd64.deb  
        sudo ldconfig # refresh library list for st-link  

## Additional Resources

Download the [STM32Cube-F3](https://www.st.com/en/embedded-software/stm32cubef3.html#get-software) Library and extract the Files to the ```~/opt``` Folder.
Check the **STM32_PATH** Definition in the Makefile and change it aproperly.

[STM32F303VC Product Page](http://www.st.com/web/catalog/mmc/FM141/SC1169/SS1576/LN1531/PF252054)  
[STM32F303xC Datasheet](http://www.st.com/st-web-ui/static/active/en/resource/technical/document/datasheet/DM00058181.pdf)  
[STM32F3 Family Reference Manual](http://www.st.com/st-web-ui/static/active/en/resource/technical/document/reference_manual/DM00043574.pdf)  
[Cortex-M4 Technical Reference Manual](http://infocenter.arm.com/help/topic/com.arm.doc.ddi0439c/DDI0439C_cortex_m4_r0p1_trm.pdf)  

## Credits
Thanks to Fabian Greif for his minimal example [repository](https://github.com/dergraaf/stm32f3_minimal) for the STM32F3-Discovery board.
I wish also to thank Matthew Blythe and 'mohammedari' for theier good startpoints:  
[Basic Template](https://github.com/mblythe86/stm32f3-discovery-basic-template)  
[Test Makefile](https://github.com/mohammedari/stm32f3discovery-test-c)  


