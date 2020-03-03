char nameDB[16][20] = {"ADHI VIARI N#",
                    "GHUFRON ZAHID M#",
                    "IBNU AMUNA M#",
                    "ISMAIL AKROMUL J#",
                    "KAMARUDIN ANHAR#",
                    "DINDA FANIASIH S#",
                    "SIGIT TRIWIBOWO#",
                    "M AZIS K#",
                    "DAVID ADIKUSUMA#",
                    "IVMA SALSABILA#",
                    "DAVIN D B#",
                    "LABIB H#",
                    "FIKRI RIDA P#",
                    "MICHAEL A#",
                    "RAVEDYA AUFA A#"};

void setup() {
  Serial.begin(9600);

  if(searchName("FIKRI RIDA P")) Serial.println("ADA"); else Serial.println("GAK ADA");
  if(searchName("FIKRI")) Serial.println("ADA"); else Serial.println("GAK ADA");
  if(searchName("CUYYYY")) Serial.println("ADA"); else Serial.println("GAK ADA");
  
}

void loop() {

}

bool searchName (String inputName){
  Serial.print("Searching for ");
  Serial.print(inputName);
  Serial.println("...");
  String nameDB_str;
  
  for(int i=0; i<15; i++){
    for(int j=0; j<16; j++){
      char c = nameDB[i][j];
      if(c == '#') break;
      nameDB_str += c;
    }
    if(nameDB_str == inputName){
      return true;
      break;
    }
    nameDB_str = "";
  }
  return false;
}

