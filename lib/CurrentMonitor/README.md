# CurrentMonitor Project

Driver class for calculating Irms from the E-Series ADC

## Usage

Add the CurrentMonitor project to your library,
include CurrentMonitor.h to your project.

```
#include "CurrentMonitor.h"
CurrentMonitor monitor(120.0, 1.65, 2000, 47, 3.3, 1480);

void loop() {
  double power = monitor.getPower(0); // change to any channel from 0-7
  double iRMS = monitor.getIrms(0);
  Serial.println(Irms);
  Serial.println(iRMS);
}
```

## LICENSE
Copyright 2019 Bachta

This project is licensed under the MIT license - see the [LICENSE.md](LICENSE.md) file for details