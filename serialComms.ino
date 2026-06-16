#include <EEPROM.h>

const int MAX_STRINGS = 10;  
const int STRING_LEN = 95;  

bool isSaving = false;
int saveIndex = 0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    input.trim();

    if (input == "CMD:LOAD") {
      for (int i = 0; i < MAX_STRINGS; i++) {
        char buffer[STRING_LEN];
        EEPROM.get(i * STRING_LEN, buffer);
        
        // Cek jika tidak kosong dan bukan memori pabrik (255)
        if (buffer[0] != '\0' && (byte)buffer[0] != 255) {
          Serial.println(buffer);
        }
      }
      Serial.println("END_LOAD");
      
      // Bersihkan EEPROM setelah di-load
      char empty[STRING_LEN] = {0};
      for (int i = 0; i < MAX_STRINGS; i++) {
        EEPROM.put(i * STRING_LEN, empty);
      }
    } 
    else if (input == "CMD:SAVE") {
      isSaving = true;
      saveIndex = 0;
    } 
    else if (input == "CMD:DONE") {
      isSaving = false;
      char empty[STRING_LEN] = {0};
      for (int i = saveIndex; i < MAX_STRINGS; i++) {
        EEPROM.put(i * STRING_LEN, empty);
      }
    } 
    else if (isSaving && saveIndex < MAX_STRINGS) {
      char buffer[STRING_LEN] = {0};
      input.toCharArray(buffer, STRING_LEN);
      EEPROM.put(saveIndex * STRING_LEN, buffer);
      saveIndex++;
    }
  }
}
