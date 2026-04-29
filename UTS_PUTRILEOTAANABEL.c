#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ESP32Servo.h>

// 1. KONFIGURASI PIN
const int PIN_HUJAN  = 35; // Sensor Hujan (Analog)
const int PIN_LDR    = 34; // Potensiometer/LDR (Analog)
const int PIN_SERVO  = 18; // Motor Servo
const int PIN_BUZZER = 27; // Buzzer
const int PIN_RED    = 12; // LED RGB (Merah)
const int PIN_GREEN  = 14; // LED RGB (Hijau)
const int PIN_BLUE   = 13; // LED RGB (Biru)

// 2. AMBANG BATAS (THRESHOLD)
const int LIMIT_HUJAN = 2000; // Semakin kecil = semakin basah
const int LIMIT_GELAP = 1500; // Semakin kecil = semakin gelap

// Inisialisasi Objek
LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo myservo;
bool jemuranAman = false; // Status posisi jemuran

void setup() {
  Serial.begin(115200);
  
  // Setup Output
  pinMode(PIN_BUZZER, OUTPUT);
  pinMode(PIN_RED, OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);
  pinMode(PIN_BLUE, OUTPUT);
  
  // Setup LCD & Servo
  lcd.init();
  lcd.backlight();
  myservo.attach(PIN_SERVO);
  
  // Tampilan Awal
  lcd.setCursor(0, 0);
  lcd.print("SYSTEM STARTING");
  digitalWrite(PIN_BLUE, HIGH); // Biru saat booting
  delay(2000);
  digitalWrite(PIN_BLUE, LOW);
  lcd.clear();
}

void loop() {
  // Baca Data Sensor
  int valHujan = analogRead(PIN_HUJAN);
  int valLDR   = analogRead(PIN_LDR);

  // Cek Kondisi (Logika OR)
  bool harusMasuk = (valHujan < LIMIT_HUJAN || valLDR < LIMIT_GELAP);

  // Update Tampilan Data
  lcd.setCursor(0, 0);
  lcd.print("H:" + String(valHujan) + " L:" + String(valLDR) + "    ");

  if (harusMasuk) {
    // AKSI: AMANKAN JEMURAN
    myservo.write(90); 
    digitalWrite(PIN_RED, HIGH);
    digitalWrite(PIN_GREEN, LOW);
    
    lcd.setCursor(0, 1);
    lcd.print("STATUS: AMANKAN!");

    // Buzzer bunyi jika baru saja berpindah status
    if (!jemuranAman) {
      bunyiPeringatan();
      jemuranAman = true;
    }
  } 
  else {
    // AKSI: KELUARKAN JEMURAN
    myservo.write(0);
    digitalWrite(PIN_RED, LOW);
    digitalWrite(PIN_GREEN, HIGH);
    
    lcd.setCursor(0, 1);
    lcd.print("STATUS: JEMUR   ");
    
    if (jemuranAman) {
      bunyiPeringatan();
      jemuranAman = false;
    }
  }

  delay(500); 
}

// Fungsi Kreativitas: Bunyi Beep 2x
void bunyiPeringatan() {
  for(int i=0; i<2; i++) {
    digitalWrite(PIN_BUZZER, HIGH);
    delay(100);
    digitalWrite(PIN_BUZZER, LOW);
    delay(100);
  }
}