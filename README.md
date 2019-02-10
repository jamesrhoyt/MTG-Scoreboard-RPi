MTG Scoreboard (RPi)
--------------------

### Installing Library Dependencies

#### SFML

```
sudo apt-get install libsfml-dev
```

#### TGUI

Install cmake: ```sudo apt-get install cmake```

Download and unzip TGUI 0.8.3: https://github.com/texus/TGUI/archive/v0.8.3.zip

Build TGUI with cmake:

```
cd /home/pi/Downloads/TGUI-0.8.3
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

### Setting Up the MySQL Server

Install Apache2:

```
sudo apt-get install apache2
sudo a2enmod rewrite
sudo service apache2 restart
```

Install PHP: ```sudo apt-get install php libapache2-mod-php```

Install and Set Up MySQL:

* ```sudo apt-get install mysql-server php-mysql```
* Choose "UNIX Socket" as the Connection Method for the MySQL database
* Set the username of the database's administrative user to "root"
* Set the password of the database's administrative user to "mtgscoreboard"
* Set the MySQL username for PHPMyAdmin to "root"
* Set the MySQL database name for PHPMyAdmin to "phpmyadmin"
* Choose "apache2" as the web server to reconfigure automatically
* If you get Error 1045, choose "Ignore"
* ```sudo service apache2 restart```

Install and Set Up PHPMyAdmin:

* ```sudo apt-get install phpmyadmin```
* Set the password for the MySQL "root" user to "mtgscoreboard"
* Confirm the password for the MySQL "root" user: "mtgscoreboard"
* ```sudo mysql_secure_installation```
* Enter the current password for "root": "mtgscoreboard"
* Choose "n" when asked to "Change the root password"
* Choose "y" when asked to "Remove anonymous users"
* Choose "y" when asked to "Disallow root login remotely"
* Choose "y" when asked to "Remove test database and access to it"
* Choose "y" when asked to "Reload privilege tables now"

Add simulinks:

```
cd /etc/apache2/conf-available
sudo ln -s ../../phpmyadmin/apache.conf phpmyadmin.conf
cd /etc/apache2/conf-enabled
sudo ln -s ../conf-available/phpmyadimn.conf phpmyadmin.conf
sudo service apache2 restart
```

Set Up the "mtg" Database and Required Tables:

```sudo mysql -u root -p```
Enter password: "mtgscoreboard"
```
CREATE DATABASE mtg /g
USE mtg
CREATE TABLE `players` (`id` int(11) NOT NULL, `name` text NOT NULL, `wins` int(11) NOT NULL DEFAULT '0', `ties` int(11) NOT NULL DEFAULT '0', `losses` int(11) NOT NULL DEFAULT '0', `text_color` varchar(6) NOT NULL, `background_color` varchar(6) NOT NULL, `avatar` text) ENGINE=InnoDB DEFAULT CHARSET=latin1;
CREATE TABLE `games` (`id` int(11) NOT NULL, `name` text NOT NULL, `active` tinyint(1) NOT NULL, `team_size` int(11) NOT NULL, `game_type` int(11) NOT NULL, `start_time` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP, `turns` int(11)  NOT NULL) ENGINE=InnoDB DEFAULT CHARSET=latin1;
CREATE TABLE `teams` (`id` int(11) NOT NULL, `team_id` int(11) NOT NULL, `name` text NOT NULL, `game_id` int(11) NOT NULL, `life` int(11) NOT NULL, `player_id` int(11) NOT NULL, `partner1_id` int(11) NOT NULL, `partner1_life` int(11) NOT NULL, `partner1_poison` int(11) NOT NULL, `partner2_id` int(11) NOT NULL, `partner2_life` int(11) NOT NULL, `partner2_poison` int(11) NOT NULL, `emperor` tinyint(1) NOT NULL, `poison` int(11) NOT NULL, `isAlive` bit NOT NULL) ENGINE=InnoDB DEFAULT CHARSET=latin1;
CREATE TABLE `commander_dmg` (`id` int(11) NOT NULL, `game_id` int(11) NOT NULL, `from_id` int(11) NOT NULL, `to_id` int(11) NOT NULL, `life` int(11) NOT NULL) ENGINE=InnoDB DEFAULT CHARSET=latin1;
ALTER TABLE `players` ADD PRIMARY KEY (`id`), ADD UNIQUE KEY `id` (`id`);
COMMIT;
ALTER TABLE `games` ADD PRIMARY KEY (`id`), ADD UNIQUE KEY `id` (`id`);
COMMIT;
ALTER TABLE `teams` ADD PRIMARY KEY (`id`), ADD UNIQUE KEY `id` (`id`);
COMMIT;
ALTER TABLE `commander_dmg` ADD PRIMARY KEY (`id`), ADD UNIQUE KEY `id` (`id`);
COMMIT;
```

Set up the Server's Web Pages: 
* Make the "/var/www/html" folder accessible:
```
sudo chown -R pi:www-data /var/www
sudo chmod u+rxw,g+rx-w,o-rwx /var/www
sudo chmod g+s /var/www
```
* Copy the 5 files in the "Server Files" directory to "/var/www/html" on the Raspberry Pi
* Go to "localhost/mtg_lobby.php"