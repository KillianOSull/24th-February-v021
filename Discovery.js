/****************************************************************************************************************************************************************/
/* 	THIS PROGRAMME WAS CREATED BY KILLIAN O SULLIVAN													*/
/*	STUDENT NUMBER: C16318096																*/
/*	SUPERVISOR NAME: FRANK DUIGNAN																*/
/*	PROJECT DESCRIPTION:																	*/
/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*																				*/
/*	THE FOLLOWING FILE READS VARIOUS VALUES SUCH AS A ROLLLING CODE AND A COUNTER FROM A MICROCONTROLLER WHEN A BUTTON IS PRESSED 				*/
/*	WHEN A READ TAKES PLACE NODEJS WILL EXECUTE A C++ FILE TO READ IN A COUNTER VALUE, READ IN THE CODE AND COMPARE CODES. IF 				*/
/*	THERE IS A MATCH OR NOT A MATCH IN THE CODES THEN THE C++ FILE SEND EITHER A 1 OR A 0. FROM THERE NODEJS WILL READ THE FILE				*/ 
/*	CALLED MATCH.TXT AND BROADCAST A SIGNAL INTO A BLUETOOTH MESH NETWORK.											*/
/*																				*/
/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*																				*/
/*	MQTT broker dashboard available at http://www.hivemq.com/demos/websocket-client/									*/
/*																				*/
/****************************************************************************************************************************************************************/

//***********Various-libaries-included******************************************//
										//
var mqtt = require('mqtt') 			//require mqtt libary		//
var fs = require('fs')				//require BLE libary		//
const { exec } = require('child_process')	//require Terminal libary	//
										//
//***********Various-libaries-included******************************************//

//**********Login-Details-for-MQTT-Broker*******************************************************//
												//
//var mqttClient  = mqtt.connect('mqtt://broker.mqttdashboard.com');				//
/*												//
var options = {					//login options for mqtt broker			//
  //port: 8000,											//
  clientId: 'mqttjs_1998',									//
  username: "Killian",										//
  password: "pass",										//
};												//
*/												//
												//
const host = 'mqtt://broker.mqttdashboard.com' 	//declaring host				//
const port = '1883'				//declaring port number				//
const clientId = `mqtt_${Math.random().toString(16).slice(3)}` //declaring client IFD		//
												//
const connectUrl = `mqtt://${host}:${port}`	//adding options to connection			//
												//
var mqttClient  = mqtt.connect(connectUrl, {	//connect to broker with following options	//
  clientId,					/* */						//
  clean: true,					/* */						//
  connectionTimeout: 4000,			/* */						//
  username: 'killian',				/* */						//
  password: 'pass',				/* */						//
  reconnectPeriod: 1000,			/* */						//
})						/* */						//
												//
var topicToPublishTo="topic/Notifications"	//Topic to publish too 				//
var topicToSubscribeTo="topic/Commands"		//Topic to subscribe too			//
												//
//**********Login-Details-for-MQTT-Broker*******************************************************//
												
//********************************************************BT-UUID-of-services-in-microbit***********************************************************************//
//												//								//
const buttonServiceOfInterestUuid = '00000001-0002-0003-0004-000000002000' 			//uuid of button service					//
const buttonACharacteristicOfInterestUuid = '00000001-0002-0003-0004-000000002001' 		//uuid of read/notify characteristic of button A service	//
const buttonBCharacteristicOfInterestUuid = '00000001-0002-0003-0004-000000002002' 		//uuid of read/notify characteristic of button B service	//
//												//								//
const counterServiceOfInterestUuid = '00000001-0002-0003-0004-000000004000' 			//uuid of counter service					//
const unlockCounterCharacteristicOfInterestUuid = '00000001-0002-0003-0004-000000004001'	//uuid of read/notify characteristic of counter			//
const lockCounterCharacteristicOfInterestUuid = '00000001-0002-0003-0004-000000004002' 		//uuid of read/notify characteristic of counter			//
//												//								//
const codeServiceOfInterestUuid = '00000001-0002-0003-0004-000000005000' 			//uuid of code service						//
const unlockCodeCharacteristicOfInterestUuid = '00000001-0002-0003-0004-000000005001' 		//uuid of read/notify characteristic of code			//
const lockCodeCharacteristicOfInterestUuid = '00000001-0002-0003-0004-000000005002' 		//uuid of read/notify characteristic of code			//
												//								//
const ledServiceOfInterestUuid = '00000001-0002-0003-0004-000000003000' 			//uuid of LED service						//
const ledCharacteristicOfInterestUuid = '00000001-0002-0003-0004-000000003001' 			//uuid of read/write characteristic of LED service		//
//												//								//
//********************************************************BT-UUID-of-services-in-microbit***********************************************************************//

//********************************************************FUNCTIONS FOR MQTT BROKER*********************************************************************************************************************//
																									//
mqttClient.on('connect', connectCallback); 													//conntecting to MQTT broker				//	
																									//
function connectCallback() {															//connect callback function upon connecting to broker	//
   console.log("connected to cloud MQTT broker");												//prints on console					//
   mqttClient.subscribe(topicToSubscribeTo, mqttSubscribeCallback);										//call error when subscribing				//
   mqttClient.publish(topicToPublishTo, '- Publish "Commands" to see all the commands available', publishCallback);				//publish a message to the topic specified above	//
   mqttClient.publish(topicToPublishTo, '- To Send commands to the security system, please publish to "topic/Commands"', publishCallback);	//publish a message to the topic specified above	//
   mqttClient.publish(topicToPublishTo, '- To Recieve Notificeations, please subscribe to "topic/Notifications"', publishCallback);		//publish a message to the topicspecified above		//
   mqttClient.publish(topicToPublishTo, '- Hello and Welcome', publishCallback);								//publish a message to the topic specified above	//
}																									//
																									//
//********************************************************Error-checking-functions**************************************************************************************//				//
function mqttSubscribeCallback(error, granted) { 								//callback error or granted when subscribing to topic	//				// 		
   	if (error) {												//if there is an error					//				//
		console.log("error subscribing to topic");							//prints error on console				//				//
	}													//							//				//
	else {													//else there is no error				//				//
		console.log("subscribed to and awaiting messages on topic '" + topicToSubscribeTo + "'");	//prints success					//				//
        }													//							//				//
}														//end of mqttSubscribeCallback function			//				//
function publishCallback(error) {    										//callback error or granted when publishing to broker	//   				//
   	if (error) {												//if there is an error					//				//		
		console.log("error publishing data");								//prints error on console				//				//
	} 													//							//				//
	else {	 												//else if there is no error				//				//
        console.log("Message is published to topic '" + topicToPublishTo+ "'");					//prints on console success on publishing to broker	//				//
        //mqttClient.end(); // Close the connection to the broker when published				//							//				//
    	}													//							//				//
}														//end of publishCallback function			//				//
//********************************************************Error-checking-functions**************************************************************************************//				//
																									//
																									//
//********************************************************FUNCTIONS FOR MQTT BROKER*********************************************************************************************************************//

//********************************************************BLE OPTIONS AND CONNECTING TO A DEVICE************************************************//
const deviceOfInterest = 'CC:0C:27:E4:90:EC'					//mac address of device						//
var ledChar;									//led char variable						//
										//								//
const main = async() => { 							//main function							//
console.log('||      NODEJS     	||	Start of programme')		//prints on console						//
mqttClient.on('message', messageEventHandler);					//handeling messages from mqtt broker				//
										//								//
										//								//
  	const {createBluetooth}=require('node-ble') 				//nodejs ble module/library					//
  	const { bluetooth, destroy } = createBluetooth()			//get bluetooth adapter						//
  	const adapter = await bluetooth.defaultAdapter() 			//get an available Bluetooth adapter				//
  	//if(!await adapter.isDiscovering()){					//								//
  	await adapter.startDiscovery() 						//using the adapter, start a device discovery session  		//
  	//}									//								//
  	console.log('||      NODEJS     	||	Discovering')		//prints on command line					//
  										//								//
  	device = await adapter.waitDevice(deviceOfInterest)			//usese devices specified mac addess from top of programme	//
  	console.log('||      NODEJS     	||	got device', await device.getAddress())			//await device.getAddress())	//
  	const deviceName = await device.getName()				//gets the name of the device					//
  	console.log('||      NODEJS     	||	got device remote name', deviceName)			//prints on command line	//
  	console.log('||      NODEJS     	||	got device user friendly name', await device.toString())	//prints on command line//
  	console.log('||      NODEJS     	||	Device: [', deviceName , '] is within range')		//prints on command line	//
										//								//
  	await adapter.stopDiscovery() 						//stops looking for devices					//
  	console.log("||      NODEJS     	||	Stopping Discovery")					//prints on command line	//
  										//								//
  	await device.connect()							//connect to the specific device				//
  	console.log("||      NODEJS     	||	Connected to device: " + deviceName)			//prints on command line	//
  										//								//
  	const gattServer = await device.gatt()					//for recieving GATT services					//
  	services = await gattServer.services()					//for getting the serices					//
	console.log("||      NODEJS     	||	services are " + services)					//prints on command line//
  										//								//
//********************************************************BLE OPTIONS AND CONNECTING TO A DEVICE************************************************//
  	
  	if (services.includes(counterServiceOfInterestUuid)) { //counter Services
  		console.log('||      NODEJS     	||	got the Counter service')
  		const primaryUnlockCounterService = await gattServer.getPrimaryService(counterServiceOfInterestUuid)	
  		const primaryLockCounterService = await gattServer.getPrimaryService(counterServiceOfInterestUuid)
	 	unlockCounterChar = await primaryUnlockCounterService.getCharacteristic(unlockCounterCharacteristicOfInterestUuid)  	
	 	lockCounterChar = await primaryLockCounterService.getCharacteristic(lockCounterCharacteristicOfInterestUuid)
	}
	
	if (services.includes(codeServiceOfInterestUuid)) { //code Services
  		console.log('||      NODEJS     	||	got the Code service')
  		const primaryUnlockCodeService = await gattServer.getPrimaryService(codeServiceOfInterestUuid)	
  		const primaryLockCodeService = await gattServer.getPrimaryService(codeServiceOfInterestUuid)
	 	unlockCodeChar = await primaryUnlockCodeService.getCharacteristic(unlockCodeCharacteristicOfInterestUuid)  	
	 	lockCodeChar = await primaryLockCodeService.getCharacteristic(lockCodeCharacteristicOfInterestUuid)
	}
	if (services.includes(ledServiceOfInterestUuid)) { //code Services
  		console.log('||      NODEJS     	||	got the LED service')
  		const primaryLedService = await gattServer.getPrimaryService(ledServiceOfInterestUuid)
	 	ledChar = await primaryLedService.getCharacteristic(ledCharacteristicOfInterestUuid)  	
	}
	
  	if (services.includes(buttonServiceOfInterestUuid)) { //Button Services
  		console.log('||      NODEJS     	||	Got the Button service')
  		const primaryButtonAService = await gattServer.getPrimaryService(buttonServiceOfInterestUuid)	
	 	buttonAChar = await primaryButtonAService.getCharacteristic(buttonACharacteristicOfInterestUuid)  
	 	console.log("||      NODEJS     	||	Characteristic flags for button A are : " + await buttonAChar.getFlags())
	 	await buttonAChar.startNotifications()
	 	console.log('||      NODEJS		||	Button A Notifications On')
	 	buttonAChar.on('valuechanged', async BtnA => {
	 		console.log('')
	 		console.log("||      NODEJS     	||	Button A pressed				|| ")
	 		const unlock_counter_val = await unlockCounterChar.readValue()
	 		unlock_counter_val_str = unlock_counter_val.toString('hex')
	 		//console.log('Unlock Counter value from NodeJS: ' + unlock_counter_val_str)
	 		
	 		const new_unlock_counter_val = unlock_counter_val_str.charAt(6) + unlock_counter_val_str.charAt(7) + unlock_counter_val_str.charAt(4) + unlock_counter_val_str.charAt(5) + unlock_counter_val_str.charAt(2) + unlock_counter_val_str.charAt(3) + unlock_counter_val_str.charAt(0) + unlock_counter_val_str.charAt(1);
	 		
	 		unlock_counter_val_int = parseInt(new_unlock_counter_val, 16)
	 		console.log('||      NODEJS     	||	Unlock interger counter from NodeJS:		|| ' + unlock_counter_val_int)

	 		fs.writeFile('unlock_counter.txt', unlock_counter_val_int, function (err) { if (err) return console.log(err); }); //writing to a file
	 		
	 		const unlock_code_val = await unlockCodeChar.readValue()
	 		change_unlock_code = unlock_code_val.toString('hex')
	 		//console.log(unlock_code_val);
	 		//process.stdout.write('||      NODEJS    	||	The Unlock Code from NodeJS is:			|| ') //code sent is as ABCDEFGH... code recieved as GHEFCDAB because of big and little endians 
	 		
	 		/*
	 		process.stdout.write(cahnge_unlock_code.charAt(6));
	 		process.stdout.write(code.charAt(7));
	 		process.stdout.write(code.charAt(4));
	 		process.stdout.write(code.charAt(5));
	 		process.stdout.write(code.charAt(2));
	 		process.stdout.write(code.charAt(3));
	 		process.stdout.write(code.charAt(0));
	 		process.stdout.write(code.charAt(1));
	 		*/
	 		
	 		const new_unlock_code_val = change_unlock_code.charAt(6) + change_unlock_code.charAt(7) + change_unlock_code.charAt(4) + change_unlock_code.charAt(5) + change_unlock_code.charAt(2) + change_unlock_code.charAt(3) + change_unlock_code.charAt(0) + change_unlock_code.charAt(1);
	 		
	 		console.log('||      NODEJS    	||	The Unlock Code from NodeJS is:			|| ' + new_unlock_code_val);
	 		
	 		fs.writeFile('unlock_code.txt', new_unlock_code_val, function (err) { if (err) return console.log(err); }); //writing to a file
	 		await new Promise(resolve => setTimeout(resolve, 1000)); //for mutexes / seamphores 
	 		
	 		exec("./unlock", (error, stdout, stderr) => {
			    if (error) {
				console.log(`error: ${error.message}`);
				return;
			    }
			    if (stderr) {
				console.log(`${stderr}`);
				return;
			    }
			    console.log(stdout);
			});
			
			
			
			try {
			  await new Promise(resolve => setTimeout(resolve, 1000));
			  const unlock_match = fs.readFileSync('unlock_match.txt', 'utf8')
			  if (unlock_match == 1) { 
			  	console.log ('||      NODEJS     	||	ITS A MATCH					|| ') 
			  	await ledChar.writeValue(Buffer.from([0x31]));
			  	await new Promise(resolve => setTimeout(resolve, 500));
			  	await ledChar.writeValue(Buffer.from([0x32]));		
			  }
			  if (unlock_match == 0) { console.log ('||      NODEJS     	||	NOT A MATCH					|| ') }
			} catch (err) {
			  console.error(err)
			}

			
			

			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
						  
		})
	 	
	} 
	if (services.includes(buttonServiceOfInterestUuid)) {
		const primaryButtonBService = await gattServer.getPrimaryService(buttonServiceOfInterestUuid)
		buttonBChar = await primaryButtonBService.getCharacteristic(buttonBCharacteristicOfInterestUuid)
		console.log("||      NODEJS     	||	characteristic flags for button B are : " + await buttonBChar.getFlags())
		await buttonBChar.startNotifications()
	 	console.log('||      NODEJS     	||	Button B Notifications On')
		buttonBChar.on('valuechanged', async BtnB => {
			console.log('')
	 		console.log("||      NODEJS     	||	Button B pressed")
	 		const lock_counter_val = await lockCounterChar.readValue()
	 		lock_counter_val_str = lock_counter_val.toString('hex')
	 		//console.log('Lock Counter value: ' + lock_counter_val_str)
	 		
	 		const new_lock_counter_val = lock_counter_val_str.charAt(6) + lock_counter_val_str.charAt(7) + lock_counter_val_str.charAt(4) + lock_counter_val_str.charAt(5) + lock_counter_val_str.charAt(2) + lock_counter_val_str.charAt(3) + lock_counter_val_str.charAt(0) + lock_counter_val_str.charAt(1);
	 		
	 		lock_counter_val_int = parseInt(new_lock_counter_val, 16)
	 		console.log('||      NODEJS     	||	Lock interget counter from NodeJS:		|| ' + lock_counter_val_int)

	 		fs.writeFile('lock_counter.txt', lock_counter_val_int, function (err) { if (err) return console.log(err); }); //writing to a file
	 		
	 		const lock_code_val = await lockCodeChar.readValue()
	 		change_lock_code = lock_code_val.toString('hex')
	 		//console.log(lock_code_val); 
	 		//process.stdout.write('||     NODEJS     ||	The Lock Code is: ') //code sent is as ABCDEFGH... code recieved as GHEFCDAB because of big and little endians 
	 		
	 		/*
	 		process.stdout.write(code.charAt(6));
	 		process.stdout.write(code.charAt(7));
	 		process.stdout.write(code.charAt(4));
	 		process.stdout.write(code.charAt(5));
	 		process.stdout.write(code.charAt(2));
	 		process.stdout.write(code.charAt(3));
	 		process.stdout.write(code.charAt(0));
	 		process.stdout.write(code.charAt(1));
	 		*/
	 		
	 		const new_lock_code_val = change_lock_code.charAt(6) + change_lock_code.charAt(7) + change_lock_code.charAt(4) + change_lock_code.charAt(5) + change_lock_code.charAt(2) + change_lock_code.charAt(3) + change_lock_code.charAt(0) + change_lock_code.charAt(1);
	 		console.log('||      NODEJS    	||	The Lock Code from NodeJS is:			|| ' + new_lock_code_val);
	 		
	 		fs.writeFile('lock_code.txt', new_lock_code_val, function (err) { if (err) return console.log(err); }); //writing to a file
	 		await new Promise(resolve => setTimeout(resolve, 1000));
	 		
	 		exec("./lock", (error, stdout, stderr) => {
			    if (error) {
				console.log(`error: ${error.message}`);
				return;
			    }
			    if (stderr) {
				console.log(`stderr: ${stderr}`);
				return;
			    }
			    console.log(stdout);
			});
			
			try {
			  await new Promise(resolve => setTimeout(resolve, 1000)); //pauses the programme
			  const lock_match = fs.readFileSync('lock_match.txt', 'utf8')
			  if (lock_match == 1) { console.log ('||      NODEJS     	||	ITS A MATCH					|| ') }
			  if (lock_match == 0) { console.log ('||      NODEJS     	||	ITS NOT A MATCH					|| ') }
			} catch (err) {
			  console.error(err)
			}
	 		
	 		
	 		
	 		
	 		
	 		
	 		
	 		
	 		
	 		
	 		
	 		
	 		
	 		
	 		
	 		
	 		
	 		
	 		
	 		
	 		
	 		
	 		
	 		
	 		
	 		
	 		
	 	})
	}
  
  
  
  
  
  
  
  
  
  
  /*
  	if(device.connect() == 'true'){
  	   console.log("connected to device : " + deviceName)
  	}							//prints on command line
  	
  	await new Promise(resolve => setTimeout(resolve, 10000))						//waits 1 min
  	
  	await device.disconnect()
  	destroy()
  	console.log('disconnected')
  */	


//***********************************Publishing-commands**********************************//
async function messageEventHandler(topic, message, packet) {
        
	if(message.toString().toLowerCase() == "commands"){
		mqttClient.publish(topicToPublishTo,"- report", publishCallback);
		mqttClient.publish(topicToPublishTo,"- arm", publishCallback);
		mqttClient.publish(topicToPublishTo,"- disarm", publishCallback);
		mqttClient.publish(topicToPublishTo,"- unlock", publishCallback);
		mqttClient.publish(topicToPublishTo,"- lock", publishCallback);
		mqttClient.publish(topicToPublishTo,"The Commands are as follows:", publishCallback);
		mqttClient.publish(topicToPublishTo,'The message Published is "Commands"', publishCallback);
	}
	else if(message.toString().toLowerCase() == "lock"){
		mqttClient.publish(topicToPublishTo,'The doors are locked"', publishCallback);
		mqttClient.publish(topicToPublishTo,'The message Published is "Lock"', publishCallback);
	}
	else if(message.toString().toLowerCase() == "unlock"){
		mqttClient.publish(topicToPublishTo,'The doors are unlocked"', publishCallback);
		mqttClient.publish(topicToPublishTo,'The message Published is "Unlock"', publishCallback);
	}
	else if(message.toString().toLowerCase() == "arm"){
		mqttClient.publish(topicToPublishTo,'The system is armed', publishCallback);
		mqttClient.publish(topicToPublishTo,'The message Published is "Arm"', publishCallback);
	}
	else if(message.toString().toLowerCase() == "disarm"){
		mqttClient.publish(topicToPublishTo,'The system is disarmed', publishCallback);
		mqttClient.publish(topicToPublishTo,'The message Published is "Disarm"', publishCallback);
	}
	else if(message.toString().toLowerCase() == "report"){
		mqttClient.publish(topicToPublishTo,'The system Report is', publishCallback);
		mqttClient.publish(topicToPublishTo,'The message Published is "Report"', publishCallback);
	}
	else{
	mqttClient.publish(topicToPublishTo,"Invalid Command", publishCallback);
	}  
   }


//***********************************Publishing-commands**********************************//	
	
} 	
main()
  .then()
   .catch(console.error)
