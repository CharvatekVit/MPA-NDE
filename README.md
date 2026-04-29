# MPA-NDE

Team members

1. Vít Charvátek (responsible for Pneumatics)
2. Vít Janoš (responsible for Electronics)
4. Antonín Putala (responsible for Software, Structure)

## Requirements
1.	Operation on battery supply
2.	Wireless control capability
2.  Attitude determination using an onboard sensor
2.  Actuation using compressed air
2.	Analysis of the pulse response of the system
2.  Maintaining a stable orientation around a single axis
2.	Performing controlled orientation changes

> [!WARNING]
> May 5 will be final presentation.

## Task list
- [ ] System
   * [x] Conceptual diagram
   * [x] Requirements
   * [ ] Energy budget
   * [x] Component list
   * [x] Component purchase
   * [x] Physical description
   * [ ] Report
- [x]  Electronics
   * [x] Component choice 
      - [x] Battery
      - [x] Step-down
      - [x] Accelerometer
   * [x] Circuit design
      - [x] LED indicator
      - [x] Switching circuit
   * [x] PCB design
   * [x] Soldering
- [x]  Pneumatics
   * [x] Component choice 
      - [x] Valve
      - [x] T-hub
      - [x] Tube
   * [x]  Gas bottle construction
   * [x]  Completation pneumatic circuit
   * [x]  Sealing
   * [x]  Nozzles
- [x] Software
   * [x] Component choice 
       - [x] MCU
       - [x] Wireless module
   * [x] UART communication
       - [x] Sensor data
       - [x] Valve control
           * [x] Position setting
           * [x] Position regulation
       - [x] Command implementation
   * [x] Wireless communication
   * [x] Constant tuning
   * [x] Testing
   * [x] Doxygen
- [x] Structure
   * [x] Size estimation
   * [x] Structural design
   * [x] 3D print
   * [x] Assembly

## Sharepoint
- [Part list](https://vutbr-my.sharepoint.com/:x:/r/personal/246858_vutbr_cz/Documents/MPA-NDE%20Part%20List.xlsx?d=wf46451cfd72a41359eeacaa9987770c3&csf=1&web=1&e=Fm6BbQ)
- [Presentation 1](https://vutbr-my.sharepoint.com/:p:/r/personal/246850_vutbr_cz/_layouts/15/Doc.aspx?sourcedoc=%7BA22FE4DB-148C-41C8-9C42-CBA38E74E679%7D&file=Prezentace_1.pptx&fromShare=true&action=edit&mobileredirect=true)
- [Presentation 2](https://vutbr-my.sharepoint.com/:p:/r/personal/246922_vutbr_cz/_layouts/15/Doc.aspx?sourcedoc=%7B1F2A7DCB-3A78-4C2B-ABEC-51CC65FE5E03%7D&file=Prezentace_2.pptx&action=edit&mobileredirect=true)
- [Presentation 3](https://vutbr-my.sharepoint.com/:p:/r/personal/246922_vutbr_cz/_layouts/15/Doc.aspx?sourcedoc=%7Bb8d8ee2d-dea4-4bd7-80e0-b8eedcd8eff3%7D&action=edit&wdPreviousSession=f2da6c50-2410-d63b-7747-ba218f665f97)
- [Presentation 4](https://vutbr-my.sharepoint.com/:p:/r/personal/246922_vutbr_cz/_layouts/15/Doc.aspx?sourcedoc=%7Bfdc04afe-ad7c-41b7-ba45-5333d5b4e1ec%7D&action=edit&wdPreviousSession=a0a396ff-f194-bd80-6fab-eeac188b319b)
- [Presentation 5](https://vutbr-my.sharepoint.com/:p:/r/personal/246922_vutbr_cz/_layouts/15/Doc.aspx?sourcedoc=%7B985cec3f-7b48-4ae2-b9dc-034aea59f2ab%7D&action=edit&wdPreviousSession=30f991b1-2b14-5247-71b9-fc82ac9b66c8)
- [Programming documantation]([https://github.com/CharvatekVit/MPA-NDE/blob/main/Documentation/html/index.html](https://raw.githubusercontent.com/CharvatekVit/MPA-NDE/main/Documentation/html/index.html)

## Reference
1. [Arduino HC-05](https://arduinogetstarted.com/tutorials/arduino-bluetooth#google_vignette)
3. [Calculator - Potencial Energy of Compression](https://codingace.net/physics/potential_energy_of_compressed.html)
4. [Calculator - Dynamic Viscosity](https://www.engineeringtoolbox.com/air-absolute-kinematic-viscosity-d_601.html)
5. [CubeSAT dimension specification](https://www.nasa.gov/wp-content/uploads/2018/01/cubesatdesignspecificationrev14_12022-02-09.pdf)
2. [MPU-9250 Datasheet](https://github.com/kriswiner/MPU9250/blob/master/Documents/RM-MPU-9250A-00.pdf)
7. [MPU-9250 Video 1](https://www.bing.com/videos/riverview/relatedvideo?q=mpu-9250%2f6500&&mid=BEA83FD97D245E639D38BEA83FD97D245E639D38&FORM=VAMGZC)
8. [MPU-9250 Video 2](https://www.youtube.com/watch?v=UEnWlSgGPiE&t=22s)
9. [MPU-9250 Library](https://github.com/libdriver/mpu9250)
10. [BluePill pinout](https://deepbluembedded.com/stm32-blue-pill-pinout-programming-guide/)
11. [C Compiler online](https://www.programiz.com/c-programming/online-compiler/)
12. [Nanosatellites Design and Electronics](https://moodle.vut.cz/pluginfile.php/1264520/mod_resource/content/1/SPACE_NDE_Povalac_202203.pdf)
