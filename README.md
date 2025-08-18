# threadedtraceroute
--**THIS REPOSITORY IS FOR DOCUMENTATION PURPOSES**-- (dont use this code, its ass)

**WORK IN PROGRESS!**

## Current Features
-   **Single IP Traceroute, all threaded, done in seconds.<br>
    -> Single probes for every unique ttl send raw packets to destination IP

## Current Focus
- **Adding Json file output Feature**
- **Better error handling (exceptions etc)**
- **Multi ip scan feature**
- **Multi scan single ip**
- **ICMP respond packet parser**
- **Addition of cmake**
---

This is version 2.0 of my traceroute project, which main focus is on improving the run time of sending/waiting for packets. 
Improvement: (simplified)<br>
->1.0 : **Hops*MAX_TIMEOUT_NOREPLY**<br>
->2.0 : **MAX_TIMEOUT_NOREPLY**

The threaded version runs max for the timeout occuring while waiting for respond packages, which probably got dropped. (1-2s)<br>
Version 1.0: [KaliTraceRoute](https://www.github.com/KaliKiu/packettraceroute)
