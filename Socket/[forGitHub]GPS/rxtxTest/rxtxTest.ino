void setup() {
  // put your setup code here, to run once:
  Serial1.begin(115200);
  while(!Serial) delay(10);

}

void loop() {
  // put your main code here, to run repeatedly:
  Seirla1.println("hi");
  delay(1000);

}
