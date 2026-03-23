# MPA-NDE

Team members

1. Vít Charvátek (responsible for Pneumatics)
2. Vít Janoš (responsible for Electronics)
4. Antonín Putala (responsible for Software, Structure)

> [!WARNING]
> Next progress presentation will be on March 30.

## Requirements
1. Determining position using a sensor
2. Compressed air drive
3. Maintaining a constant position on one axis
4. Instruction for rotation of 90°
5. Battery supply
6. Wireless control

## Task list
- [ ] System
   * [x] Conceptual diagram
   * [x] Requirements
   * [ ] Energy budget
   * [x] Component list
   * [x] Component purchase
   * [x] Physical description
- [ ]  Electronics
   * [x] Component choice 
      - [x] Battery
      - [x] Step-down
      - [x] Accelerometer
   * [x] Circuit design
      - [x] LED indicator
      - [x] Switching circuit
   * [x] PCB design
   * [ ] Soldering
- [ ]  Pneumatics
   * [x] Component choice 
      - [x] Valve
      - [x] T-hub
      - [x] Tube
   * [x]  Gas bottle construction
   * [ ]  Completation pneumatic circuit
- [ ] Software
   * [x] Component choice 
       - [x] MCU
       - [x] Wireless module
   * [ ] UART communication
       - [ ] Sensor data
       - [ ] Valve control
           * [ ] Position setting
           * [ ] Position regulation
       - [x] Command implementation
   * [x] Wireless communication
- [ ] Structure
   * [x] Size estimation
   * [x] Structural design
   * [x] 3D print
   * [ ] Assembly

## Sharepoint
- [Part list](https://vutbr-my.sharepoint.com/:x:/r/personal/246858_vutbr_cz/Documents/MPA-NDE%20Part%20List.xlsx?d=wf46451cfd72a41359eeacaa9987770c3&csf=1&web=1&e=Fm6BbQ)
- [Presentation 1](https://vutbr-my.sharepoint.com/:p:/r/personal/246850_vutbr_cz/_layouts/15/Doc.aspx?sourcedoc=%7BA22FE4DB-148C-41C8-9C42-CBA38E74E679%7D&file=Prezentace_1.pptx&fromShare=true&action=edit&mobileredirect=true)
- [Presentation 2](https://vutbr-my.sharepoint.com/:p:/r/personal/246922_vutbr_cz/_layouts/15/Doc.aspx?sourcedoc=%7B1F2A7DCB-3A78-4C2B-ABEC-51CC65FE5E03%7D&file=Prezentace_2.pptx&action=edit&mobileredirect=true)

## Reference
1. [Arduino HC-05](https://arduinogetstarted.com/tutorials/arduino-bluetooth#google_vignette)
2. [Calculator - Potencial Energy of Compression](https://codingace.net/physics/potential_energy_of_compressed.html)
3. [Calculator - Dynamic Viscosity](https://www.engineeringtoolbox.com/air-absolute-kinematic-viscosity-d_601.html)
4. [CubeSAT dimension specification](https://www.nasa.gov/wp-content/uploads/2018/01/cubesatdesignspecificationrev14_12022-02-09.pdf)
5. [MP-9250 Video 1](https://www.bing.com/videos/riverview/relatedvideo?q=mpu-9250%2f6500&&mid=BEA83FD97D245E639D38BEA83FD97D245E639D38&FORM=VAMGZC)
6. [MP-9250 Video 2](https://www.youtube.com/watch?v=UEnWlSgGPiE&t=22s)
7. [MP-9250 Library](https://github.com/libdriver/mpu9250)
8. [BluePill pinout](https://deepbluembedded.com/stm32-blue-pill-pinout-programming-guide/)
