;*******************************************************************************
;*                Système d'Injection et d'Allumage Électronique               *
;*                Version     : 01                                             *
;*                Auteur      : Hugo Villeneuve                                *
;*                                                                             *
;*                10 mars 1999                                                 *
;*******************************************************************************

        $MOD52                  ; Micro-contrôleur Atmel AT89S8252


;*******************************************************************************
;*      Définition des constantes                                              *
;*******************************************************************************

TOS             EQU     60h     ; Adresse du dessus de la pile.
CR              EQU     0Dh	; Code ASCII pour un retour de chariot.
LF              EQU     0Ah	; Code ASCII pour un changement de ligne


;*******************************************************************************
;*      Définition des variables                                               *
;*******************************************************************************
                BSEG
                ORG     20h
C_FLAG:         DBIT    1
Z_FLAG:         DBIT    2
N_FLAG:		DBIT	3	; Utilisé par la sous-routine MULT8_16
SIGNE:		DBIT	4	; Utilisé pour l'interpolation.

                DSEG
                ORG     30h
PERIODE2:	DS	1	; Période pour une rotation du vilebrequin, sur 24 bits.
PERIODE1:       DS      1
PERIODE0:       DS      1
POSITION_VILB:	DS	1	; Renseignement sur la position actuelle du vilebrequin (zones 0,1 ou 2).
VITESSE_RPM:	DS	2	; Vitesse de rotation du moteur en RPM.
INDEX_RPM:	DS	1	; Index de 8 bits pour l'adressage des colonnes de la table d'allumage.
INDEX_MAP:	DS	1	; Index de 8 bits pour l'adressage des lignes de la table d'allumage.
ANGLE:		DS	1	; Angle d'allumage calculé à partir de la table.
BAT:            DS      2       ; Voltage de la batterie.
MAT:            DS      2       ; Manifold Air Temperature.
CLT:            DS      2       ; Coolant Temperature.
TPS:            DS      2       ; Throttle Position Sensor.
MAP:            DS      2       ; Manifold Absolute Pressure.
EGO:            DS      2       ; Exhaust Gas-Oxygen Sensor.
CAN6:           DS      2       ; Canal #6 du convertisseur AN.
CAN7:           DS      2       ; Canal #7 du convertisseur AN.
GAMMA:          DS      2       ; Rapport Air/Carburant.
LSB_CAN:        DS      1       ; Octet de poids faible de la conversion Analogique-Numérique.
MSB_CAN:        DS      1       ; Octet de poids fort de la conversion Analogique-Numérique.
NOMBRE4:        DS      1       ; Stockage des codes ASCII pour les conversions.
NOMBRE3:        DS      1
NOMBRE2:        DS      1
NOMBRE1:        DS      1
NOMBRE0:        DS      1
C3:             DS      1       ; Accumulateur C de 32 bits pour les calculs mathématiques.
C2:             DS      1
C1:             DS      1
C0:             DS      1
D3:		DS	1       ; Accumulateur D de 32 bits pour les calculs mathématiques.
D2:		DS	1
D1:             DS      1
D0:             DS      1
TMP6:           DS      1       ; Variables temporaires utilisées pour les calculs mathématiques.
TMP5:           DS      1	; FAIRE LE MÉNAGE VARIABLES NON UTILISEES!!!
TMP4:           DS      1
TMP3:           DS      1
TMP2:           DS      1
TMP1:           DS      1
TMP0:           DS      1
VAR0:		DS	1
VAR1:		DS	1
VAR2:		DS	1


;*******************************************************************************
;*      Définition des régistres spécifiques au AT89S8252                      *
;*******************************************************************************
SPCR            DATA    0D5h            ; SPCR - SPI Control Register
SPSR            DATA    0AAh            ; SPSR - SPI Status Register
SPIF            EQU     10000000b       ; Masque pour le drapeau SPI.
WCOL            EQU     01000000b       ; Masque pour le drapeau Write Collision.
SPDR            DATA    086h            ; SPDR - SPI Data Register


;*******************************************************************************
;*      Vecteurs d'interruptions                                               *
;*******************************************************************************
        CSEG

        ORG     0000h   ; Vecteur d'interruption du RESET.
        JMP     DEBUT

        ORG     0003h   ; Vecteur pour l'interruption EXTERNE 0.
        JMP     VILEBREQUIN

        ORG     000Bh   ; Vecteur pour l'interruption du TIMER 0.
        JMP     DEBUT

        ORG     0013h   ; Vecteur pour l'interruption EXTERNE 1.
        JMP     DEBUT

        ORG     001Bh   ; Vecteur pour l'interruption du TIMER 1.
        JMP     DEBUT

        ORG     0023h   ; Vecteur pour l'interruption du Port série.
        JMP     DEBUT

        ORG     002Bh   ; Vecteur pour l'interruption du TIMER 2.
        JMP     DEBUT


;*******************************************************************************
;*      Début du programme principal                                           *
;*******************************************************************************
        ORG     0033h

DEBUT:
        MOV     SP,#TOS         ; Initialisation de la pile.
        CALL    INITIALISATION
        ; il ne faut pas modifier la valeur de P1.0!!!

ICI:
	NOP
	NOP
	NOP
	NOP	

        JMP     ICI


;*******************************************************************************
;*      Délai                                                                  *
;*******************************************************************************
DELAI:          MOV     TMP0,#016h      ; Délai de 1/2 seconde.
B3:		MOV	A,#0	
B2:		MOV	B,#0
B1:		DJNZ	B,B1
		DJNZ	ACC,B2	
                DJNZ    TMP0,B3  
                RET



;*******************************************************************************
;*      INTERRUPTION                                                           *
;*******************************************************************************
VILEBREQUIN:
	RETI


;*******************************************************************************
;*      Initialisation                                                         *
;*******************************************************************************
INITIALISATION: MOV     SPCR,#01000100b ; Interruption SPI désactivée;
					; Activation du port SPI;
					; Ordre des transferts : MSB en premier;
					; Opération en mode escalve (SLAVE);
                                        ; Polarité de l'horloge : niveau bas si inactif.
					; Phase de l'horloge : transfert sur front montant.
	MOV	TH0,#2
	MOV	TL0,#55h
	SETB   	TCON.4		; Timer 0 ON.
	SETB   	TCON.6		; Timer 1 ON.
	MOV     TMOD,#00010001B ; Initialisation des timers 0 et 1 en timers de
				; 16 bits, incrémentés par l'horloge interne
				; Chaque timer est incrémenté tant que le bit correspondant de
				; TCON est à 1 (TCON.4 et TCON.6).
	RET



        END





