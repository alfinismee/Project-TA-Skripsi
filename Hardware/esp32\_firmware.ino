#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define SENSOR_PIN 26
#define SDA_PIN    21
#define SCL_PIN    22

LiquidCrystal_I2C lcd(0x27, 16, 2);

int jumlahLele            = 0;
bool objekTerdeteksi      = false;
bool sedangDikunci        = false;
unsigned long lastDeteksi = 0;
unsigned long waktuMasuk  = 0;
unsigned long waktuKunci  = 0;
int highCount = 0;
int lowCount  = 0;

// --- Tuning ---
const unsigned long DURASI_MIN_MS    = 25;
const unsigned long DURASI_MAX_MS    = 450;
const unsigned long WINDOW_LOCK_MS   = 600;
const unsigned long AIR_THRESHOLD_MS = 350;
const int SAMPLE_COUNT = 2;

byte ikanKiri[8] = {
  0b00000, 0b00110, 0b01111,
  0b11111, 0b11111, 0b01111,
  0b00110, 0b00000
};
byte ikanKanan[8] = {
  0b00000, 0b01100, 0b11110,
  0b11111, 0b11111, 0b11110,
  0b01100, 0b00000
};

void setup() {
  Serial.begin(115200);
  Wire.begin(SDA_PIN, SCL_PIN);
  lcd.begin();
  lcd.backlight();
  lcd.createChar(0, ikanKiri);
  lcd.createChar(1, ikanKanan);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.write(byte(0));
  lcd.write(byte(1));
  lcd.print(" LELE COUNTER");
  lcd.setCursor(0, 1);
  lcd.print("  Siap hitung...");

  pinMode(SENSOR_PIN, INPUT_PULLUP);

  // === STABILISASI AWAL ===
  // Biarkan sensor stabil dulu selama 3 detik
  // sambil tampilkan countdown di LCD
  for (int i = 3; i > 0; i--) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.write(byte(0));
    lcd.write(byte(1));
    lcd.print(" Stabilisasi...");
    lcd.setCursor(0, 1);
    lcd.print("Siap dalam ");
    lcd.print(i);
    lcd.print(" detik");
    delay(1000);
  }

  // Baca sensor beberapa kali dulu supaya stabil
  for (int i = 0; i < 20; i++) {
    digitalRead(SENSOR_PIN);
    delay(10);
  }

  // Reset semua counter setelah stabilisasi
  highCount       = 0;
  lowCount        = 0;
  objekTerdeteksi = false;
  sedangDikunci   = false;

  tampilkanLayar();

  Serial.println("=== Penghitung Bibit Lele ===");
  Serial.println("Sensor stabil, siap menghitung!");
}

void loop() {
  int statusSensor = digitalRead(SENSOR_PIN);
  unsigned long sekarang = millis();

  // Masa kunci aktif - abaikan semua sinyal
  if (sedangDikunci) {
    if (sekarang - waktuKunci >= WINDOW_LOCK_MS) {
      sedangDikunci   = false;
      highCount       = 0;
      lowCount        = 0;
      objekTerdeteksi = false;
    } else {
      return;
    }
  }

  // Sampling sinyal
  if (statusSensor == LOW) {
    lowCount++;
    highCount = 0;
  } else {
    highCount++;
    lowCount = 0;
  }

  // Bibit mulai menghalangi → LED berkedip/mati = LOW
  if (lowCount >= SAMPLE_COUNT && !objekTerdeteksi) {
    objekTerdeteksi = true;
    waktuMasuk      = sekarang;
    lowCount        = 0;
  }

  // Bibit sudah lewat → LED menyala lagi = HIGH
  if (highCount >= SAMPLE_COUNT && objekTerdeteksi) {
    unsigned long durasi = sekarang - waktuMasuk;

    if (durasi >= DURASI_MIN_MS && durasi <= DURASI_MAX_MS) {
      jumlahLele++;
      lastDeteksi   = sekarang;
      waktuKunci    = sekarang;
      sedangDikunci = true;
      tampilkanLayar();

    } else if (durasi > DURASI_MAX_MS) {
      objekTerdeteksi = false;
      highCount       = 0;

    } else {
      objekTerdeteksi = false;
      highCount       = 0;
    }
  }

  // Reset otomatis kalau LOW terlalu lama
  if (objekTerdeteksi && (sekarang - waktuMasuk > AIR_THRESHOLD_MS)) {
    objekTerdeteksi = false;
    lowCount        = 0;
    highCount       = 0;
  }

  delay(1);
}

void tampilkanLayar() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.write(byte(0));
  lcd.write(byte(1));
  lcd.print(" Bibit Lele:");
  lcd.setCursor(0, 1);
  lcd.print("Jumlah: ");
  lcd.print(jumlahLele);
  lcd.print(" ekor");
}
