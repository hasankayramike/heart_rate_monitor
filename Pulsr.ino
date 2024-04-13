//SH1106 ekran ve pulseSensor kullandim

#include "U8glib.h" //Ekranin kutuphanesini ekledim
#define USE_ARDUINO_INTERRUPTS true //Nabiz olcumlerinin dogru yapilmasi icin true olarak ayarladim
#include <PulseSensorPlayground.h> //Nabiz sensorunun kutuphanesini ekledim

int bpm = 0; //Nabiz degiskenimi olusturdum
const int PulseWire = A0; //Nabiz sensorunun bağlandigi analog pinini tutan degiskeni olusturdum 
const int LED13 = 13; //Nabizla birlikte yanip sonmesini saglayacagım ledi tutan degiskeni olusturdum
const int Threshold = 550; //Nabiz olcumundeki esik degerini belirttim
unsigned long timeUpdate = 0; //Iki olcum arasinda gececek zamani belirtecegim degiskenlerimi yarattim 
unsigned long timeNow;


PulseSensorPlayground pulseSensor; //Sensorun kutuphanesini kullanacak degiskenimi tanimladim

U8GLIB_SSD1306_128X64 SSD1306_Display; //Fiziksel boyutlari uygun ekranin kutuphnesini kullanacak degiskenimi tanimladim 


//Ekrandaki gorseli olustuacak fonksiyon, parametre olarak olculen nabiz degerini alir
void DrawHeartBeat(int pulseBPM)
{
  char szBpm[10];
  SSD1306_Display.setFont (u8g_font_courB12); //Kullanacagim fontu belirledim
  SSD1306_Display.drawLine(1,1,128,1); //Ekranin en tepesinde x ekseni boyunca bir cizgi cektim
  SSD1306_Display.drawLine(1,15, 128,15); //y = 15, x = 1->128 seklinde bir cizgi cektim
  SSD1306_Display.drawStr (35, 13 , " Pulsr "); //Cektigim cizgilerin arasina projemin adini yazdim
  SSD1306_Display.drawStr (35,42 , "BPM:"); //y=15 cizgisinin altina ekrani ortalayacak sekilde BPM: yazdim
  sprintf(szBpm, "%d", pulseBPM); //Nabzimi ekranda yazilabilir bir degiskene atadim  
  SSD1306_Display.drawStr(75,42, szBpm); //Ekranda "BPM:" yazisinin yanina nabiz degerimi yazdim

}

void setup(void)
{   
  SSD1306_Display.setFont(u8g_font_courB12); //Fontumu belirledim
  Serial.begin(9600); //Serial monitorumu baslattim
 
  pulseSensor.analogInput(PulseWire); //Nabiz sesnorunun bagli bulundugu analog pinini belirttim  
  pulseSensor.blinkOnPulse(LED13); //Nabizla birlikte arduino uzerinde yanacak ledi belirttim
  pulseSensor.setThreshold(Threshold); //Sensore esik degerini attım   

  if (pulseSensor.begin()) Serial.println("pulseSensor objesini olusturduk!");  //Sensorun algilanip algilanmadigini sorgular. Algilandiysa serial monitore ilgili cumleyi yazar  
}

void loop(void)
{
  timeNow = millis(); //Anlik zamani bir degiskene atadim
  if ((timeNow - timeUpdate) < 500) return; //Nabiz son olculdugunden bu yana en az 0.5 saniye gectiyse bir daha olcum yap
  timeUpdate = timeNow; //Su anki zamani son olcum zamani olarak kullan

  bpm = pulseSensor.getBeatsPerMinute(); //Dakikadaki nabiz atim sayimizi olcup bunu bir degiskene atiyoruz

  //Nabiz esik degerini astiysa ledi yak  
  if (bpm < Threshold) digitalWrite(LED13, LOW);
  else digitalWrite(LED13, HIGH);
  
  SSD1306_Display.firstPage(); //Ekrani sonsuz donguye aldim
  do
  {
    DrawHeartBeat(bpm); //Ekrandaki nabiz degerini guncelle
    Serial.print("BPM: "); //Serial monitore "BPM: nabiz" yaz  
    Serial.println(bpm);
  } while (SSD1306_Display.nextPage());
}
