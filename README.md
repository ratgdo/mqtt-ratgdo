# ratgdo

ratgdo gives you **local** MQTT & dry contact control plus status feedback for your residential Chamberlain/LiftMaster/Merlin garage door opener _including_ Security+ 2.0 openers. (Security+ 2.0 uses an encrypted serial signal to control the door opener's open/close and light functions, which makes it impossible to use Shelly One, Go Control, Insteon I/O linc or any other dry contact relay device to control the door). 

ratgdo wires to your door opener's terminals and restores dry contact control. It also allows you to control the door with MQTT over your local WiFi network which can be used to integrate directly with NodeRED or Home Assistant, eliminating the need for another "smart" device.

ratgdo is *not* a cloud device and does *not* require a subscription service. The firmware is open source and free for anyone to use.

ratgdo also can control door openers that use a dry contact interface. Status feedback is provided through two reed switches.

> **ratgdo available to order**
> Shipping domestic USA via USPS or internaltional via Global Post.
>
> * [ratgdo v2.5 control board only](https://square.link/u/B5pW7OZW)
> * [ratgdo v2.5 control board with installation kit](https://square.link/u/FKqlMSWT)

# [Visit the github.io page for instructions](https://paulwieland.github.io/ratgdo/).
[ratgdo on GitHub.io](https://paulwieland.github.io/ratgdo/)

# To Do
* [ ] Eliminate dependence on arduinobootstrapper
* [ ] Implement web serial (requires different web server library)
* [x] Add mqtt toggle commands for control commands
* [x] Add secplus1 lock commands
* [x] Fix secplus1 light control bug
* [ ] Implement auto tune for obstruction sensors
* [x] Require two of the same status message for security + 1.0 to eliminate false reports from GDO
* [ ] Flip light status on when motion detected packet is sent

# Special thanks

Special thanks to kosibar, Brad and TechJunkie01 - without whom this project would not have been possible.
