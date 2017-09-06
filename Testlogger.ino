//////////////////////////////////////////////////////////////////////
// **************************************************************** //
//	SD card datalogger												//
//	This example code is in the public domain.						//
// **************************************************************** //	 
//////////////////////////////////////////////////////////////////////

#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <RTClib.h>

const int chipSelect = 4;
RTC_DS1307 RTC;

//char LaDate         [22];                 // Variable regroupant la date et l'heure.
//char nomfichier       [8];                // Permet le stockage du nom du fichier avec horodatage.
		//  ---   ---       Variables SD        ---   --- //
	File fichier;                         // Crée un objet fichier.
	//File racine;                          // Crée un objet racine.
//////////////////////////////////////////////////////////////////////
// **************************************************************** //
// *********************** Progamme Principal ********************* //
// **************************************************************** //
//////////////////////////////////////////////////////////////////////
void setup()
{
	Serial.begin(9600);
	Serial.println("TEST LOGGER");
	pinMode(SS, OUTPUT);
	Wire.begin();
		Serial.println("Wire init ok");
	RTC.begin();
		Serial.println("rtc init ok");
	//designationfichier(); 											// Appel du sous-programme créant le nom du fichier.
	//SDInit(chipSelect); 
	Serial.println("file init ok");

	if (!SD.begin(chipSelect)) {
		Serial.println("SD failed, or not present");
		while (1) ;
	}
	Serial.println("card initialized.");
	
	// Open up the file we're going to log to!
	fichier = SD.open("datalog.txt", FILE_WRITE);
	if (! fichier) {
		Serial.println("error opening file");
		// Wait forever since we cant write data
		while (1) ;
	}
	SDcardInit();
}
//////////////////////////////////////////////////////////////////////
// **************************************************************** //
// ************************* Progamme Boucle ********************** //
// **************************************************************** //
//////////////////////////////////////////////////////////////////////
void loop()
{
	String dataString = readAna();
	char *dateh = readRTC();
	// SERIAL
	Serial.print(dateh);
	Serial.print(",\t");
	Serial.println(dataString);  // print to the serial port too:
	// FICHIER
	fichier.print(dateh);
	fichier.print(",\t");
	fichier.println(dataString);
	fichier.flush();			//fait une sauvegarde des datas tout les 512b

	delay(1000);			// OHHHHH MAIS C'EST DE LA MERDE CA !!!!
}
//////////////////////////////////////////////////////////////////////
// **************************************************************** //
// ************************* Programme SD ************************* //
// **************************************************************** //
//////////////////////////////////////////////////////////////////////
void SDcardInit()
{
	// fichier.println("//--------- OctoGeeK - testSD V5 ---------//");// Entête.
	fichier.println("Date - heure ; ana1; ana2; ana3");// Format du tableau.
}
//////////////////////////////////////////////////////////////////////
// **************************************************************** //
// *******************  Progamme Lecture ANA ********************** //
// **************************************************************** //
//////////////////////////////////////////////////////////////////////
String readAna()
{
	String dataString = "";
	for (int analogPin = 0; analogPin < 3; analogPin++) {
		int sensor = analogRead(analogPin);
		dataString += String(sensor);
		if (analogPin < 2) {
			dataString += ";"; 
		}
	}
	return dataString;
}
//////////////////////////////////////////////////////////////////////
// **************************************************************** //
// *******************  Progamme Lecture RTC ********************** //
// **************************************************************** //
//////////////////////////////////////////////////////////////////////
char * readRTC()
{
	char LaDate         [22];                 // Variable regroupant la date et l'heure.
	DateTime now  = RTC.now();                  // Retourne l'horodatage du RTC.
	int secondes    = now.second();                 // Variable des secondes.
	int minutes   = now.minute();                 // Variable des minutes.
	int heures    = now.hour();                 // Variable des heures.
	int jour      = now.day();                  // Variable du jour du mois.
	int mois      = now.month();                  // Variable des mois.
	int annee     = now.year();                 // Variable des années.
	sprintf(LaDate,"%02d/%02d/%04d - %02d:%02d:%02d",jour,mois,annee,heures,minutes,secondes);
	return LaDate;
}
