// Defines the stations of the Expo and Millennium Lines of the SkyTrain network.
// Please note that Commercial-Broadway is split into Broadway and Commercial.
// Each station is assigned an int ID in their data order.

#ifndef _MKIII_STATIONS_H
#define _MKIII_STATIONS_H

#define NUM_STATIONS 39

#define STN_WATERFRONT        0
#define STN_BURRARD           1
#define STN_GRANVILLE         2
#define STN_STADIUM           3
#define STN_MAIN_STREET       4
#define STN_BROADWAY          5
#define STN_NANAIMO           6
#define STN_29TH_AVENUE       7
#define STN_JOYCE             8
#define STN_PATTERSON         9
#define STN_METROTOWN         10
#define STN_ROYAL_OAK         11
#define STN_EDMONDS           12
#define STN_22ND_STREET       13
#define STN_NEW_WESTMINSTER   14
#define STN_COLUMBIA          15
#define STN_SCOTT_ROAD        16
#define STN_GATEWAY           17
#define STN_SURREY_CENTRAL    18
#define STN_KING_GEORGE       19
#define STN_LAFARGE           20
#define STN_LINCOLN           21
#define STN_COQUITLAM_CENTRAL 22
#define STN_INLET_CENTRE      23
#define STN_MOODY_CENTRE      24
#define STN_BURQUITLAM        25
#define STN_BRAID             26
#define STN_SAPPERTON         27
#define STN_LOUGHEED          28
#define STN_PRODUCTION        29
#define STN_LAKE_CITY_WAY     30
#define STN_SPERLING          31
#define STN_HOLDOM            32
#define STN_BRENTWOOD         33
#define STN_GILMORE           34
#define STN_RUPERT            35
#define STN_RENFREW           36
#define STN_COMMERCIAL        37
#define STN_VCC_CLARK         38

// Below: Defines the LED orderings.
// The data order is the actual data order of LEDs for each station from head to tail.
// This is also the same numeric order of the stations as defined in stations.h.
// The x order is the left-to-right order of LEDs.

const uint8_t STATION_DATA_ORDER[NUM_STATIONS] = {
  STN_WATERFRONT,
  STN_BURRARD,
  STN_GRANVILLE,
  STN_STADIUM,
  STN_MAIN_STREET,
  STN_BROADWAY,
  STN_NANAIMO,
  STN_29TH_AVENUE,
  STN_JOYCE,
  STN_PATTERSON,
  STN_METROTOWN,
  STN_ROYAL_OAK,
  STN_EDMONDS,
  STN_22ND_STREET,
  STN_NEW_WESTMINSTER,
  STN_COLUMBIA,
  STN_SCOTT_ROAD,
  STN_GATEWAY,
  STN_SURREY_CENTRAL,
  STN_KING_GEORGE,
  STN_LAFARGE,
  STN_LINCOLN,
  STN_COQUITLAM_CENTRAL,
  STN_INLET_CENTRE,
  STN_MOODY_CENTRE,
  STN_BURQUITLAM,
  STN_BRAID,
  STN_SAPPERTON,
  STN_LOUGHEED,
  STN_PRODUCTION,
  STN_LAKE_CITY_WAY,
  STN_SPERLING,
  STN_HOLDOM,
  STN_BRENTWOOD,
  STN_GILMORE,
  STN_RUPERT,
  STN_RENFREW,
  STN_COMMERCIAL,
  STN_VCC_CLARK
};

const uint8_t STATION_X_ORDER[NUM_STATIONS] = {
  STN_WATERFRONT,
  STN_BURRARD,
  STN_GRANVILLE,
  STN_STADIUM,
  STN_VCC_CLARK,
  STN_MAIN_STREET,
  STN_BROADWAY,
  STN_COMMERCIAL,
  STN_NANAIMO,
  STN_RENFREW,
  STN_29TH_AVENUE,
  STN_RUPERT,
  STN_JOYCE,
  STN_GILMORE,
  STN_PATTERSON,
  STN_BRENTWOOD,
  STN_METROTOWN,
  STN_HOLDOM,
  STN_ROYAL_OAK,
  STN_SPERLING,
  STN_EDMONDS,
  STN_LAKE_CITY_WAY,
  STN_22ND_STREET,
  STN_PRODUCTION,
  STN_NEW_WESTMINSTER,
  STN_LOUGHEED,
  STN_COLUMBIA,
  STN_BURQUITLAM,
  STN_BRAID,
  STN_SAPPERTON,
  STN_SCOTT_ROAD,
  STN_MOODY_CENTRE,
  STN_GATEWAY,
  STN_INLET_CENTRE,
  STN_SURREY_CENTRAL,
  STN_COQUITLAM_CENTRAL,
  STN_KING_GEORGE,
  STN_LINCOLN,
  STN_LAFARGE
};

#endif
