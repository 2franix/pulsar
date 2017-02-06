# pulsar
Minimalistic CLI for monitoring a PulseAudio server.

It is used to probe samples that are output by a sink. The process returns 0 (success) if samples could be detected before timeout is reached.

It is aimed at being used as a atomic utility in a more complex script. 

# Installation
* Download latest version [here](https://github.com/2franix/pulsar/archive/1.0.0b.zip)
* Unzip archive
* In the root directory of the inflated archive, run `./configure && make install`

# Example
```
pulsar && echo "Default Pulseaudio server's default sink is playing sound!"
```

# Command line options
Allowed options:
```
  --server arg                    Server to connect to.
  -s [ --sink ] arg               Sink to connect to.
  -t [ --timeout ] arg (=0.5)     Probing duration in seconds. It is the 
                                  maximum amount of time before the program 
                                  will return a non-zero value if no sample 
                                  could be probed.
  -v [ --verbosity ] arg (=error) Verbose mode. Value must be in [info, 
                                  warning, error].
  -h [ --help ]                   Print this message.
```
