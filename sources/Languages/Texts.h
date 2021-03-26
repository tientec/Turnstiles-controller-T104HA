const uint8_t * Polish_Tab[] = {
"TIENTEC",/*20*/
"Gate Controller",/*20*/
"Ver.   .  .",/*20*/
"Pn",/*3*/
"Wt",/*3*/
"Sr",/*3*/
"Cz",/*3*/
"Pt",/*3*/
"So",/*3*/
"Nd",/*3*/
"--------MENU--------",/*20*/
"-----Parametry------",/*20*/
"-----Ustawienia-----",/*20*/
"Fun. dodatkowe",/*20*/
"Wybor bramki",/*20*/
"Data i czas",/*20*/
"Oprogramowanie",/*20*/
"Test/Serwis/Czujniki",/*20*/
"Param. sterowania",/*18*/
"Ustaw. sterownika",/*18*/
"Funkcje dodatkowe",/*18*/
"Wybor bramki",/*18*/
"Data i czas",/*18*/
"Oprogramowanie",/*18*/
"Test/Serwis/Czuj.",/*18*/
"Przywroc Ust.Fabr.",/*18*/
"Inf. o sterowniku",/*18*/
"Mechanizm",/*18*/
"Sterowanie",/*18*/
"Ogrz./Went.",/*18*/
"Param. mech.",/*20*/
"Param. ster.",/*20*/
"Ust. hasel",/*18*/
"Ust. blokady",/*18*/
"Kalibracja RTC",/*18*/
"Wybor jezyka",/*18*/
"Zer. licznikow",/*18*/
"Wprow. haslo dostepu",/*20*/
"Wprow. haslo blokady",/*20*/
"Sterownik",/*20*/
"Zablokowany",/*20*/
"Blokada calkowita",/*20*/
"Data blokady",/*12*/
"Pozostalo prob:",/*16*/
"Licencja programu:",/*20*/
"Bezterminowo",/*20*/
"Do dnia:",/*8*/
"<OK> - Odblokuj",/*20*/
"Polski   1/2",/*12*/
"English  2/2",/*12*/
"Tak",/*3*/
"Nie",/*3*/
"Wlaczone",/*9*/
"Wylaczone",/*9*/
">",/*x*/
"<",/*x*/
".  .       :",/*x*/
".   -   .",/*x*/
".",/*x*/
":",/*x*/
"L.Cewka Rew.",/*12*/
"P.Cewka Rew.",/*12*/
"Kolowrot",/*12*/
"Hamulec",/*12*/
"Rygle",/*12*/
"Reg. Zbijaka",/*12*/
"Reg. L.Cewki",/*12*/
"Reg. P.Cewki",/*12*/
"Reg. Hamulca",/*12*/
"Predk.Siln.We",/*14*/
"Predk.Siln.Wy",/*14*/
"SAP",/*13*/
"Wolne wyj.",/*13*/
"Pamiec",/*13*/
"Tak-NO",/*6*/
"Tak-NC",/*6*/
"Lewo",/*6*/
"Prawo",/*6*/
"Swobodny",/*6*/
"Brak",/*6*/
"Czas otw.[s]",/*14*/
"Sygnal zwr.[s]",/*16*/
"3",/*3*/
"4",/*3*/
"We.",/*3*/
"Wy.",/*3*/
"Haslo",/*5*/
"Proby",/*5*/
"Dostepu",/*7*/
"Blokady",/*7*/
"Blokada",/*8*/
"Data",/*8*/
"Prog. testowy",/*18*/
"Test wejsc/wyjsc",/*18*/
"Ust. enkodera",/*18*/
"<OK> - START/STOP",/*20*/
"<OK> - Tak",/*20*/
"<ESC> - Nie",/*20*/
"HR2-1",/*x*/
"HR2-2",/*x*/
"HR3-1/2-3",/*x*/
"HR3-1/2-4",/*x*/
"HR3-STAD",/*x*/
"HR2-BASIC",/*x*/
"HR2-GA",/*x*/
"--T:   C--",/*x*/
"SAP",/*3*/
"WW",/*3*/
"PAM",/*3*/
"BLO",/*3*/
"START",/*x*/
"STOP",/*x*/
"Ust. serwisowe",/*20*/
"Odczyt param.",/*20*/
"1/",/*x*/
"2/",/*x*/
"3/",/*x*/
"4/",/*x*/
"5/",/*x*/
"1",/*x*/
"2",/*x*/
"3",/*x*/
"4",/*x*/
"5",/*x*/
"Ogrz.",/*5*/
"Went.",/*5*/
"Min",/*3*/
"Max",/*3*/
"Param. Ogrz./Went.",
"Silnik",
"Prz.czasowy/W.wyj.",
"Prz.czasowy/Sz.wej.",
"Sz.Wej./Hamulec",
"Sz.Wej./Czas",
"P1        P4",
"P2        P5",
"P3        P6",
"P7        P10",
"P8        P11",
"P9        P12",
"P13       P16",
"P14       P17",
"P15       P18",
"P19       P22",
"P20       P23",
"P21       P24",
"Prior. przej.",
"P",
"Oswietlenie",
"Prz.czas.",
"Czujnik",
"Czujnik",
"Enkoder",
"Indukcyjny",
"Typ Syg. zwr.",/*13*/
"Stand.",/*6*/
"Drzwi",/*6*/
"Czas przejscia[s]",/*17*/
"Kat wlaczenia",/*17*/
"Kat hamowania",/*17*/
"Test czuj. induk.",
"Test wyjsc MOSFET",
"Test wyjsc OC",
"Test przekaznikow",
"Test weentylatora",
"Test wejsc",
"We1",
"We2",
"We3",
"We4",
"We5",
"We6",
"We7",
"We8",
"Wersja:    .  .",
"Data:   .  .",
"N/S:",
"We. Czasowe",
"Czas sygn.[s]",
"Czas Wol.Wyj.[s]",
"Tamper[s]",
"Sensory:",
"---  ",
"TEST ",
"OK   ",
"BLAD!",
};

const uint8_t * English_Tab[] = {
"TIENTEC",
"Gate Controller",
"Ver.   .  .",
"Mo",
"Tu",
"We",
"Th",
"Fr",
"Sa",
"Su",
"--------MENU--------",
"-----Parameters-----",
"------Settings------",
"Functions",
"Select gate type",
"Date and time",
"Firmware",
"Test/Service/Sensors",
"Parameters",
"Settings",
"Other functions",
"Gate type",
"Date and time",
"Firmware",
"Test/Service/Sens.",
"Rest.Fact.Sets",
"Controller info",/*18*/
"Mechanism param.",
"Control param.",
"Heat/Cool",
"Mech. param.",
"Control param.",
"Passwords",
"Lock settings",
"RTC calibration",
"Language select",
"Clear counters",
"Enter menu password",
"Enter lock password",
"Controller",
"Blocked",
"Full blockade",
"Blockade date",
"Number of trials:",
"Firmware licence",
"Unlimited",
"Till day:",
"<OK> - Unlock",
"Polski   1/2",
"English  2/2",
"Yes",
"No ",
"Enabled",
"Disabled",
">",
"<",
".  .       :",
".   -   .",
".",
":",
"L.Coil Rev.",
"R.Coil Rev.",
"Windlass",
"Brake",
"Bolts",
"Coil",
"PWM-L.Coil",
"PWM-R.Coil",
"PWM-Brake",
"Mo. speed in",
"Mo. speed out",
"FPS",
"Free exit",
"Memory",
"Yes-NO",
"Yes-NC",
"Left",
"Right",
"Both",
"None",
"Open time[s]",
"Back signal[s]",
"3",
"4",
"In",
"Out",
"Password",
"Trials",
"Access",
"Locks",
"Lock",
"Data",
"Test program",
"Inp./outputs test",
"Encoder set",
"<OK> - START/STOP",
"<OK> - Yes",
"<ESC> - No",
"HR2-1",
"HR2-2",
"HR3-1/2-3",
"HR3-1/2-4",
"HR3-STAD",
"HR2-BASIC",
"HR2-GA",
"--T:   C--",
"FPS",
"FE",
"MEM",
"BLO",
"START",
"STOP",
"Service sets",
"Show param.",
"1/",
"2/",
"3/",
"4/",
"5/",
"1",
"2",
"3",
"4",
"5",
"Heat.",
"Fan",
"Min",
"Max",
"Fan/Heat param.",
"Motor",
"Interval/Free Out.",
"Interval/Fast pass.",
"Fast pass/Brake",
"Fast pass/Inter.",
"P1        P4",
"P2        P5",
"P3        P6",
"P7        P10",
"P8        P11",
"P9        P12",
"P13       P16",
"P14       P17",
"P15       P18",
"P19       P22",
"P20       P23",
"P21       P24",
"Priority",
"P",
"Lights",
"Period",
"Sensor",
"Sensor",
"Encoder",
"Inductive",
"Back sig.type",/*13*/
"Stand.",/*6*/
"Door",/*6*/
"Trans. time[s]",/*17*/
"Motor on angle",/*17*/
"Braking angle",/*17*/
"Ind. sensor test",/*18*/
"MOSFETs test",
"OC outputs test",
"Relays Test",
"Fan test",
"Inputs test",
"In1",
"In2",
"In3",
"In4",
"In5",
"In6",
"In7",
"In8",
"Version:   .  .",
"Date:   .  .",
"S/N:",
"Time input",
"Signal time[s]",
"Free exit t.[s]",
"Tamper relay[s]",
"Sensors:",
"---  ",
"TEST ",
"OK   ",
"FALSE",
};