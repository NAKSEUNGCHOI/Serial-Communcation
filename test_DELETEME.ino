/**
 * @file test_DELETEME.ino
 * @author Nakseung Choi @ UW
 * @date Jun-17-2022
 * @brief Running Free RTOS in Ubuntu Virtualbox
 */
#include <Arduino_FreeRTOS.h>
#include <dht_nonblocking.h>
#define DHT_SENSOR_TYPE DHT_TYPE_11

static const int DHT_SENSOR_PIN = 2;
DHT_nonblocking dht_sensor( DHT_SENSOR_PIN, DHT_SENSOR_TYPE );

void setup( ){
  Serial.begin( 9600);
  while(!Serial){;}
  xTaskCreate(TaskLED, "LED", 128, NULL, 2, NULL);
  xTaskCreate(TaskTemperature, "DHT11", 500, NULL, 1, NULL);
  vTaskStartScheduler();
}
void loop(){
  // keep empty
}

void TaskLED( void *pvParameters ){
  LED_setup();
  for(;;){
    digitalWrite(12, HIGH);
    vTaskDelay( 250 / portTICK_PERIOD_MS );
    digitalWrite(12, LOW);
    vTaskDelay( 250 / portTICK_PERIOD_MS );
  }
}
void LED_setup(){
  pinMode(12, OUTPUT);
}

static bool measure_environment( float *temperature, float *humidity )
{
  static unsigned long measurement_timestamp = millis( );

  /* Measure once every four seconds. */
  if( millis( ) - measurement_timestamp > 1000ul )
  {
    if( dht_sensor.measure( temperature, humidity ) == true )
    {
      measurement_timestamp = millis( );
      return( true );
    }
  }

  return( false );
}

void TaskTemperature( void *pvParameters ){
  TemperatureSetup();
  float temperature;
  float humidity;
  unsigned long count = 0;

  /* Measure temperature and humidity.  If the functions returns
     true, then a measurement is available. */
  while(1){
    if( measure_environment( &temperature, &humidity ) == true )
    {
      count++;
      Serial.print(count);
      Serial.print(". ");
      Serial.print( "T = " );
      Serial.print( temperature, 1 );
      Serial.print( " deg. C, H = " );
      Serial.print( humidity, 1 );
      Serial.println( "%" );
      if(count == 10000){
        count = 0;
      }
    }
  }
}

void TemperatureSetup(){
  Serial.begin(9600);
}
