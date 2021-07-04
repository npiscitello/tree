EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Device:R R3
U 1 1 5FBBA5F6
P 2800 3750
F 0 "R3" V 2593 3750 50  0001 C CNN
F 1 "10" V 2800 3750 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0309_L9.0mm_D3.2mm_P2.54mm_Vertical" V 2730 3750 50  0001 C CNN
F 3 "~" H 2800 3750 50  0001 C CNN
	1    2800 3750
	0    1    1    0   
$EndComp
$Comp
L Device:Q_PMOS_GDS Q1
U 1 1 5FBBB09F
P 2500 3500
F 0 "Q1" H 2705 3454 50  0001 L CNN
F 1 "PMOS" H 2700 3500 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-220-3_Horizontal_TabDown" H 2700 3600 50  0001 C CNN
F 3 "~" H 2500 3500 50  0001 C CNN
	1    2500 3500
	1    0    0    1   
$EndComp
$Comp
L Device:Q_NMOS_GDS Q2
U 1 1 5FBBBE00
P 2500 4100
F 0 "Q2" H 2704 4146 50  0001 L CNN
F 1 "NMOS" H 2700 4100 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-220-3_Horizontal_TabDown" H 2700 4200 50  0001 C CNN
F 3 "~" H 2500 4100 50  0001 C CNN
	1    2500 4100
	1    0    0    -1  
$EndComp
Wire Wire Line
	2600 3900 2600 3850
Wire Wire Line
	2600 3700 2600 3750
Wire Wire Line
	2600 3750 2650 3750
$Comp
L power:GND #PWR0101
U 1 1 5FBBF043
P 2600 4550
F 0 "#PWR0101" H 2600 4300 50  0001 C CNN
F 1 "GND" H 2605 4377 50  0000 C CNN
F 2 "" H 2600 4550 50  0001 C CNN
F 3 "" H 2600 4550 50  0001 C CNN
	1    2600 4550
	1    0    0    -1  
$EndComp
Wire Wire Line
	2600 4550 2600 4500
Connection ~ 2600 3100
Wire Wire Line
	3100 3750 3150 3750
Wire Wire Line
	2950 3750 3000 3750
Wire Wire Line
	2900 3850 2600 3850
Connection ~ 2600 3850
Wire Wire Line
	3000 3850 3200 3850
Wire Wire Line
	3100 3950 2900 3950
Wire Wire Line
	2900 3850 2900 3950
Connection ~ 2600 4500
Wire Wire Line
	2300 4100 2250 4100
$Comp
L MCU_Microchip_ATmega:ATmega328-PU U1
U 1 1 5FC46A39
P 5250 3700
F 0 "U1" H 4606 3746 50  0001 R CNN
F 1 "ATmega328-PU" H 5400 3700 50  0000 R CNN
F 2 "Package_DIP:DIP-28_W7.62mm" H 5250 3700 50  0001 C CIN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/ATmega328_P%20AVR%20MCU%20with%20picoPower%20Technology%20Data%20Sheet%2040001984A.pdf" H 5250 3700 50  0001 C CNN
	1    5250 3700
	-1   0    0    -1  
$EndComp
Wire Wire Line
	2250 3500 2300 3500
Wire Wire Line
	2600 3850 2250 3850
Wire Wire Line
	2250 3850 2250 3500
$Comp
L Device:R R1
U 1 1 5FC49738
P 2250 3300
F 0 "R1" V 2043 3300 50  0001 C CNN
F 1 "33k" V 2250 3300 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0309_L9.0mm_D3.2mm_P5.08mm_Vertical" V 2180 3300 50  0001 C CNN
F 3 "~" H 2250 3300 50  0001 C CNN
	1    2250 3300
	-1   0    0    1   
$EndComp
Wire Wire Line
	2250 3150 2250 3100
Wire Wire Line
	2250 3100 2600 3100
Wire Wire Line
	2600 3100 2600 3300
Wire Wire Line
	2250 3500 2250 3450
Connection ~ 2250 3500
$Comp
L Device:R R2
U 1 1 5FC5038D
P 2250 4300
F 0 "R2" V 2043 4300 50  0001 C CNN
F 1 "33k" V 2250 4300 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0309_L9.0mm_D3.2mm_P5.08mm_Vertical" V 2180 4300 50  0001 C CNN
F 3 "~" H 2250 4300 50  0001 C CNN
	1    2250 4300
	-1   0    0    1   
$EndComp
Wire Wire Line
	2250 4150 2250 4100
Connection ~ 2250 4100
Wire Wire Line
	2250 4100 2150 4100
Wire Wire Line
	2250 4450 2250 4500
Wire Wire Line
	2250 4500 2600 4500
$Comp
L power:+5V #PWR0102
U 1 1 5FC65F6D
P 5250 2100
F 0 "#PWR0102" H 5250 1950 50  0001 C CNN
F 1 "+5V" H 5265 2273 50  0000 C CNN
F 2 "" H 5250 2100 50  0001 C CNN
F 3 "" H 5250 2100 50  0001 C CNN
	1    5250 2100
	1    0    0    -1  
$EndComp
Wire Wire Line
	5250 2100 5250 2150
Connection ~ 5250 2150
Wire Wire Line
	5250 2150 5250 2200
$Comp
L power:GND #PWR0103
U 1 1 5FC6C1BE
P 5250 5250
F 0 "#PWR0103" H 5250 5000 50  0001 C CNN
F 1 "GND" H 5255 5077 50  0000 C CNN
F 2 "" H 5250 5250 50  0001 C CNN
F 3 "" H 5250 5250 50  0001 C CNN
	1    5250 5250
	1    0    0    -1  
$EndComp
Wire Wire Line
	5250 5250 5250 5200
Wire Wire Line
	5250 2150 5150 2150
Wire Wire Line
	5150 2150 5150 2200
Wire Wire Line
	5900 2150 5900 2500
Wire Wire Line
	5900 2500 5850 2500
Wire Wire Line
	5250 2150 5900 2150
Wire Wire Line
	2600 4500 2600 4300
Wire Wire Line
	2150 4100 2150 4800
Connection ~ 3500 4500
Wire Wire Line
	3850 4500 3500 4500
Wire Wire Line
	3850 4450 3850 4500
Wire Wire Line
	3850 4150 3850 4100
$Comp
L Device:R R6
U 1 1 5FC551E1
P 3850 4300
F 0 "R6" V 3643 4300 50  0001 C CNN
F 1 "33k" V 3850 4300 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0309_L9.0mm_D3.2mm_P5.08mm_Vertical" V 3780 4300 50  0001 C CNN
F 3 "~" H 3850 4300 50  0001 C CNN
	1    3850 4300
	-1   0    0    1   
$EndComp
Connection ~ 3500 3100
Wire Wire Line
	3850 3100 3500 3100
Wire Wire Line
	3850 3150 3850 3100
Connection ~ 3850 3500
Wire Wire Line
	3850 3500 3850 3450
$Comp
L Device:R R5
U 1 1 5FC4E56C
P 3850 3300
F 0 "R5" V 3643 3300 50  0001 C CNN
F 1 "33k" V 3850 3300 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0309_L9.0mm_D3.2mm_P5.08mm_Vertical" V 3780 3300 50  0001 C CNN
F 3 "~" H 3850 3300 50  0001 C CNN
	1    3850 3300
	-1   0    0    1   
$EndComp
Wire Wire Line
	3800 4100 3850 4100
Wire Wire Line
	2600 4500 3500 4500
Wire Wire Line
	3200 3850 3500 3850
Connection ~ 3500 3850
$Comp
L Device:R R4
U 1 1 5FBCAF92
P 3300 3750
F 0 "R4" V 3093 3750 50  0001 C CNN
F 1 "10" V 3300 3750 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0309_L9.0mm_D3.2mm_P2.54mm_Vertical" V 3230 3750 50  0001 C CNN
F 3 "~" H 3300 3750 50  0001 C CNN
	1    3300 3750
	0    -1   1    0   
$EndComp
$Comp
L Device:Q_PMOS_GDS Q3
U 1 1 5FBCAF9C
P 3600 3500
F 0 "Q3" H 3805 3454 50  0001 L CNN
F 1 "PMOS" H 3800 3500 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-220-3_Horizontal_TabDown" H 3800 3600 50  0001 C CNN
F 3 "~" H 3600 3500 50  0001 C CNN
	1    3600 3500
	-1   0    0    1   
$EndComp
$Comp
L Device:Q_NMOS_GDS Q4
U 1 1 5FBCAFA6
P 3600 4100
F 0 "Q4" H 3804 4146 50  0001 L CNN
F 1 "NMOS" H 3800 4100 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-220-3_Horizontal_TabDown" H 3800 4200 50  0001 C CNN
F 3 "~" H 3600 4100 50  0001 C CNN
	1    3600 4100
	-1   0    0    -1  
$EndComp
Wire Wire Line
	3500 3900 3500 3850
Wire Wire Line
	3500 3850 3850 3850
Wire Wire Line
	3850 3850 3850 3500
Wire Wire Line
	3850 3500 3800 3500
Wire Wire Line
	3500 3700 3500 3750
Wire Wire Line
	3500 3750 3450 3750
Wire Wire Line
	3500 4500 3500 4300
Wire Wire Line
	3500 3100 3500 3300
Wire Wire Line
	3500 3100 2600 3100
$Comp
L Switch:SW_SPST SW2
U 1 1 5FC774B1
P 4250 1750
F 0 "SW2" H 4250 1985 50  0001 C CNN
F 1 "POWER" H 4250 1893 50  0000 C CNN
F 2 "" H 4250 1750 50  0001 C CNN
F 3 "~" H 4250 1750 50  0001 C CNN
	1    4250 1750
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_SPDT_MSM SW1
U 1 1 5FC77FDA
P 3400 2800
F 0 "SW1" H 3400 3085 50  0001 C CNN
F 1 "MODE" H 3400 2993 50  0000 C CNN
F 2 "" H 3400 2800 50  0001 C CNN
F 3 "~" H 3400 2800 50  0001 C CNN
	1    3400 2800
	1    0    0    -1  
$EndComp
$Comp
L Connector:Screw_Terminal_01x02 J3
U 1 1 5FC7E2E1
P 4550 1750
F 0 "J3" H 4468 1967 50  0001 C CNN
F 1 "PWR" H 4468 1875 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Horizontal" H 4550 1750 50  0001 C CNN
F 3 "~" H 4550 1750 50  0001 C CNN
	1    4550 1750
	-1   0    0    -1  
$EndComp
Wire Wire Line
	4750 1750 4800 1750
$Comp
L power:GND #PWR0104
U 1 1 5FC80843
P 4800 1900
F 0 "#PWR0104" H 4800 1650 50  0001 C CNN
F 1 "GND" H 4805 1727 50  0000 C CNN
F 2 "" H 4800 1900 50  0001 C CNN
F 3 "" H 4800 1900 50  0001 C CNN
	1    4800 1900
	1    0    0    -1  
$EndComp
Wire Wire Line
	4800 1900 4800 1850
Wire Wire Line
	4800 1850 4750 1850
$Comp
L power:+5V #PWR0105
U 1 1 5FC821E7
P 4800 1650
F 0 "#PWR0105" H 4800 1500 50  0001 C CNN
F 1 "+5V" H 4815 1823 50  0000 C CNN
F 2 "" H 4800 1650 50  0001 C CNN
F 3 "" H 4800 1650 50  0001 C CNN
	1    4800 1650
	1    0    0    -1  
$EndComp
$Comp
L Connector:Screw_Terminal_01x02 J1
U 1 1 5FC848C3
P 3000 4300
F 0 "J1" H 2918 4517 50  0001 C CNN
F 1 "LED" V 3100 4250 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Horizontal" H 3000 4300 50  0001 C CNN
F 3 "~" H 3000 4300 50  0001 C CNN
	1    3000 4300
	0    -1   1    0   
$EndComp
Wire Wire Line
	3000 4100 3000 4000
Wire Wire Line
	3000 4000 2900 4000
Wire Wire Line
	2900 4000 2900 3950
Connection ~ 2900 3950
Wire Wire Line
	3100 4100 3100 4000
Wire Wire Line
	3100 4000 3200 4000
Wire Wire Line
	3200 4000 3200 3850
Connection ~ 3200 3850
Wire Wire Line
	3000 3750 3000 3850
Wire Wire Line
	3100 3750 3100 3950
Wire Wire Line
	2150 4800 4650 4800
Wire Wire Line
	3850 4100 3950 4100
Connection ~ 3850 4100
Wire Wire Line
	4050 2900 3750 2900
Wire Wire Line
	3600 2700 3650 2700
$Comp
L power:GND #PWR0106
U 1 1 5FCE281C
P 3050 2850
F 0 "#PWR0106" H 3050 2600 50  0001 C CNN
F 1 "GND" H 3055 2677 50  0000 C CNN
F 2 "" H 3050 2850 50  0001 C CNN
F 3 "" H 3050 2850 50  0001 C CNN
	1    3050 2850
	1    0    0    -1  
$EndComp
Wire Wire Line
	3050 2850 3050 2800
Wire Wire Line
	3050 2800 3200 2800
$Comp
L Device:R_POT RV1
U 1 1 5FC7ADA4
P 2750 2600
F 0 "RV1" V 2543 2600 50  0001 C CNN
F 1 "L2.5k" V 2750 2600 50  0000 C CNN
F 2 "" H 2750 2600 50  0001 C CNN
F 3 "~" H 2750 2600 50  0001 C CNN
	1    2750 2600
	1    0    0    -1  
$EndComp
$Comp
L Device:R_POT RV2
U 1 1 5FC75351
P 3050 2350
F 0 "RV2" V 2843 2350 50  0001 C CNN
F 1 "L2.5k" V 3050 2350 50  0000 C CNN
F 2 "" H 3050 2350 50  0001 C CNN
F 3 "~" H 3050 2350 50  0001 C CNN
	1    3050 2350
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0107
U 1 1 5FCF5137
P 2600 2100
F 0 "#PWR0107" H 2600 1950 50  0001 C CNN
F 1 "+5V" H 2615 2273 50  0000 C CNN
F 2 "" H 2600 2100 50  0001 C CNN
F 3 "" H 2600 2100 50  0001 C CNN
	1    2600 2100
	1    0    0    -1  
$EndComp
Wire Wire Line
	3050 2200 3050 2150
Wire Wire Line
	3050 2150 2750 2150
Wire Wire Line
	2750 2750 2750 2800
Wire Wire Line
	2750 2800 3050 2800
Wire Wire Line
	3050 2500 3050 2800
Wire Wire Line
	2900 2600 3200 2600
Wire Wire Line
	3200 2600 3200 2450
Connection ~ 3050 2800
Wire Wire Line
	2750 2150 2750 2450
Wire Wire Line
	3200 2450 3450 2450
Wire Wire Line
	3200 2350 3350 2350
Text Notes 2700 2950 0    50   ~ 0
Speed
Text Notes 3150 2250 0    50   ~ 0
Brightness
Wire Wire Line
	2600 2100 2600 2150
Wire Wire Line
	2750 2150 2600 2150
Connection ~ 2750 2150
Connection ~ 2600 2150
Wire Wire Line
	2600 2150 2600 3100
Text Label 4600 4400 2    50   ~ 0
fade
Text Label 4600 4500 2    50   ~ 0
blink
Text Label 4600 4700 2    50   ~ 0
led_B
Text Label 4600 4800 2    50   ~ 0
led_A
Wire Wire Line
	3950 4700 4650 4700
Wire Wire Line
	3950 4100 3950 4700
Wire Wire Line
	4050 4500 4050 2900
Wire Wire Line
	4650 4500 4050 4500
Wire Wire Line
	4150 4400 4650 4400
Wire Wire Line
	4150 2700 4150 4400
Wire Wire Line
	4250 3500 4650 3500
Wire Wire Line
	4250 2450 4250 3500
Wire Wire Line
	4350 3400 4650 3400
Wire Wire Line
	4350 2350 4350 3400
Text Label 4600 3500 2    50   ~ 0
speed
Text Label 4600 3400 2    50   ~ 0
bright
$Comp
L Connector:Screw_Terminal_01x06 J2
U 1 1 5FCD4E8C
P 3350 1600
F 0 "J2" V 3314 1212 50  0001 R CNN
F 1 "panel" V 3500 1600 50  0000 R CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x06_P2.54mm_Horizontal" H 3350 1600 50  0001 C CNN
F 3 "~" H 3350 1600 50  0001 C CNN
	1    3350 1600
	0    -1   -1   0   
$EndComp
Text Notes 3050 1200 0    50   ~ 0
1 - GND\n2 - +5V\n3 - brightness\n4 - speed\n5 - fade\n6 - blink
Wire Wire Line
	3050 2150 3250 2150
Wire Wire Line
	3250 2150 3250 1800
Connection ~ 3050 2150
$Comp
L power:GND #PWR0108
U 1 1 5FCD936A
P 3150 1850
F 0 "#PWR0108" H 3150 1600 50  0001 C CNN
F 1 "GND" H 3155 1677 50  0000 C CNN
F 2 "" H 3150 1850 50  0001 C CNN
F 3 "" H 3150 1850 50  0001 C CNN
	1    3150 1850
	1    0    0    -1  
$EndComp
Wire Wire Line
	3150 1850 3150 1800
Wire Wire Line
	3350 1800 3350 2350
Connection ~ 3350 2350
Wire Wire Line
	3350 2350 4350 2350
Wire Wire Line
	3450 1800 3450 2450
Connection ~ 3450 2450
Wire Wire Line
	3450 2450 4250 2450
Wire Wire Line
	3550 1800 3550 2300
Wire Wire Line
	3550 2300 3650 2300
Wire Wire Line
	3650 2300 3650 2700
Connection ~ 3650 2700
Wire Wire Line
	3650 2700 4150 2700
Wire Wire Line
	3650 1800 3650 2200
Wire Wire Line
	3650 2200 3750 2200
Wire Wire Line
	3750 2200 3750 2900
Connection ~ 3750 2900
Wire Wire Line
	3750 2900 3600 2900
Wire Wire Line
	4800 1650 4800 1750
$EndSCHEMATC
