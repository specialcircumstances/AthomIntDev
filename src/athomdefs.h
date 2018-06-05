// A number of definitions lifted from the Athom libs


#ifndef athomdefs_h
#define athomdefs_h

const bool ATHOMDEFS_DEBUG = true;

const int MAX_NUMBER_NODES = 8;
// At the moment these three MUST be the same
const int MAX_CHARS_CLASS = 32;
const int MAX_CHARS_CAPABILITY = 32;
const int MAX_CHARS_NAME = 32;
//const int HOMEYVARBYTELIMIT = 622;


// From
// https://github.com/athombv/node-homey-lib/blob/master/assets/device/classes.json
const char athomClasses[][MAX_CHARS_CLASS] = {
  "other",
  "socket",
  "light",
  "vacuumcleaner",
  "fan",
  "heater",
  "thermostat",
  "sensor",
  "kettle",
  "coffeemachine",
  "homealarm",
  "speaker",
  "button",
  "doorbell",
  "lock",
  "windowcoverings",
  "tv",
  "amplifier",
  "curtain",
  "blinds",
  "sunscreen"
};

// FROM
// https://github.com/athombv/node-homey-lib/blob/master/assets/capability/capabilities.json
const char athomCapabilities[][MAX_CHARS_CAPABILITY] {
  "onoff",
  "dim",
  "light_hue",
  "light_saturation",
  "light_temperature",
  "light_mode",
  "vacuumcleaner_state",
  "thermostat_mode",
  "target_temperature",
  "measure_temperature",
  "measure_co",
  "measure_co2",
  "measure_pm25",
  "measure_humidity",
  "measure_pressure",
  "measure_noise",
  "measure_rain",
  "measure_wind_strength",
  "measure_wind_angle",
  "measure_gust_strength",
  "measure_gust_angle",
  "measure_battery",
  "measure_power",
  "measure_voltage",
  "measure_current",
  "measure_luminance",
  "measure_ultraviolet",
  "measure_water",
  "alarm_generic",
  "alarm_motion",
  "alarm_contact",
  "alarm_co",
  "alarm_co2",
  "alarm_pm25",
  "alarm_tamper",
  "alarm_smoke",
  "alarm_fire",
  "alarm_heat",
  "alarm_water",
  "alarm_battery",
  "alarm_night",
  "meter_power",
  "meter_water",
  "meter_gas",
  "meter_rain",
  "homealarm_state",
  "volume_set",
  "volume_up",
  "volume_down",
  "volume_mute",
  "channel_up",
  "channel_down",
  "locked",
  "lock_mode",
  "windowcoverings_state",
  "windowcoverings_tilt_up",
  "windowcoverings_tilt_down",
  "windowcoverings_tilt_set",
  "windowcoverings_closed",
  "button",
  "speaker_playing",
  "speaker_next",
  "speaker_prev"
};

#endif
