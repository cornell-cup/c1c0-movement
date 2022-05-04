
uint32_t base = 0x40184000;
//uint32_t *base = 4;
uint32_t off1  = 0xc000;
uint32_t off2 = 0x18;
volatile uint32_t* Tenable = (uint32_t*)(base + off1 + 0x10);
//uint32_t reg = *Tenable;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial4.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
 *Tenable |= (1<<3);
 //*Tenable = 2;
 delay(10);
 Serial.print("Reg value = ");
 Serial.println(*Tenable,HEX);
 //*Tenable &= ~(1<<29);
 delay(100);
 Serial.print("Reg value = ");
 Serial.println(*Tenable,HEX);
}
