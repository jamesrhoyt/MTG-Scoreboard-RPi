MTG Scoreboard (RPi)
--------------------

### Installing Library Dependencies

#### SFML

```
sudo apt-get install libsfml-dev
```

#### TGUI

Install cmake: ```sudo apt-get install cmake```

Download and unzip TGUI 0.7.8: https://github.com/texus/TGUI/archive/v0.7.8.zip

Build TGUI with cmake:

```
cd /home/pi/Downloads/TGUI-0.7.8
cmake .
make
sudo make install
```

#### Boost

```
sudo apt-get install libboost1.62-all
```

#### MySQL C++ Connector

```
sudo apt-get install libmysqlcppconn-dev
```