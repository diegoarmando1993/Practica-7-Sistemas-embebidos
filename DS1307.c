//////////////////////////////////////////////////////////////////////////////// 
///                               DS1307.C                                   /// 
///                     Driver for Real Time Clock                           /// 
///                                                                          /// 
/// ds1307_init() - Enable oscillator without clearing the seconds register -/// 
///                 used when PIC loses power and DS1307 run from 3V BAT     /// 
///               - Disable squarewave output                                /// 
///                                                                          /// 
/// setTime(day,mth,year,dow,hour,min,sec)  Set the date/time   /// 
///                                                                          /// 
/// getDate(day,mth,year,dow)               Get the date             /// 
///                                                                          /// 
/// getTime(hr,min,sec)                     Get the time             /// 
///                                                                          /// 
//////////////////////////////////////////////////////////////////////////////// 

#define RTC_SDA  PIN_C4 
#define RTC_SCL  PIN_C3 

#use i2c(master, sda=RTC_SDA, scl=RTC_SCL) 

int bin2bcd(int); 
int bcd2bin(int); 

void init(void) 
{ 
   int seconds = 0; 

   i2c_start(); 
   i2c_write(0xD0);      // WR to RTC 
   i2c_write(0x00);      // REG 0 
   i2c_start(); 
   i2c_write(0xD1);      // RD from RTC 
   seconds = bcd2bin(i2c_read(0)); // Read current "seconds" in DS1307 
   i2c_stop(); 
   seconds &= 0x7F; 

   delay_us(3); 

   i2c_start(); 
   i2c_write(0xD0);      // WR to RTC 
   i2c_write(0x00);      // REG 0 
   i2c_write(bin2bcd(seconds));     // Start oscillator with current "seconds value 
   i2c_start(); 
   i2c_write(0xD0);      // WR to RTC 
   i2c_write(0x07);      // Control Register 
   i2c_write(0x80);     // Disable squarewave output pin 
   i2c_stop(); 

} 

void setTime(int day, int mth, int year, int dow, int hr, int min, int sec) 
{ 
  sec &= 0x7F; 
  hr &= 0x3F; 

  i2c_start(); 
  i2c_write(0xD0);            // I2C write address 
  i2c_write(0x00);            // Start at REG 0 - Seconds 
  i2c_write(bin2bcd(sec));      // REG 0 
  i2c_write(bin2bcd(min));      // REG 1 
  i2c_write(bin2bcd(hr));      // REG 2 
  i2c_write(bin2bcd(dow));      // REG 3 
  i2c_write(bin2bcd(day));      // REG 4 
  i2c_write(bin2bcd(mth));      // REG 5 
  i2c_write(bin2bcd(year));      // REG 6 
  i2c_write(0x80);            // REG 7 - Disable squarewave output pin 
  i2c_stop(); 
} 

void getDate(int &day, int &mth, int &year, int &dow) 
{ 
  i2c_start(); 
  i2c_write(0xD0); 
  i2c_write(0x03);            // Start at REG 3 - Day of week 
  i2c_start(); 
  i2c_write(0xD1); 
  dow  = bcd2bin(i2c_read() & 0x7f);   // REG 3 
  day  = bcd2bin(i2c_read() & 0x3f);   // REG 4 
  mth  = bcd2bin(i2c_read() & 0x1f);   // REG 5 
  year = bcd2bin(i2c_read(0));            // REG 6 
  i2c_stop(); 
} 

void getTime(int &hr, int &min, int &sec) 
{ 
  i2c_start(); 
  i2c_write(0xD0); 
  i2c_write(0x00);            // Start at REG 0 - Seconds 
  i2c_start(); 
  i2c_write(0xD1); 
  sec = bcd2bin(i2c_read() & 0x7f); 
  min = bcd2bin(i2c_read() & 0x7f); 
  hr  = bcd2bin(i2c_read(0) & 0x3f); 
  i2c_stop(); 

} 

int bin2bcd(int binary_value) 
{ 
   int decena;
   int unidad;
   int bdc = 0;
   decena = binary_value/10;
   unidad = binary_value - (decena*10);
   bdc=decena;
   bdc<<=4;
   bdc = bdc | unidad;
   return bdc;
   
} 


// Input range - 00 to 99. 
int bcd2bin(int bcd) 
{ 
   int decena;
   int unidad;
   int binary;
   
   decena = bcd;
   decena >>=4;
   decena = decena * 10;
   
   unidad = bcd & 0x0f;
   
   binary = unidad + decena;
   return binary;
} 
