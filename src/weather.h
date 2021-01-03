#ifndef WEATHER_H
#define WEATHER_H

#include <QString>

/**
 * @brief Structure WeatherData
 *
 * Stock des données météorologiques avec leurs dates de relevé.
 */
struct WeatherData {
     /**
      * @brief Date à laquelle les données ont été enregistrées.
      *
      * Format : yyyy-MM-dd hh:mm:ss
      */
     QString timestamp;

     /**
      * @brief température relevée
      */
     double temp;

     /**
      * @brief pression relevée
      */
     double press;

     /**
      * @brief humidité relevée
      */
     double hum;
};

#endif // WEATHER_H
