


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
//												//								//
//********************************************************BT-UUID-of-services-in-microbit***********************************************************************//

//********************************************************BLE OPTIONS AND CONNECTING TO A DEVICE************************************************//
const deviceOfInterest = 'CC:0C:27:E4:90:EC'					//mac address of device						//
										//								//
const main = async() => { 							//main function							//
console.log('||      NODEJS     	||	Start of programme')		//prints on console						//
//mqttClient.on('message', messageEventHandler);				//handeling messages from mqtt broker				//
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
var count = 0;

	if (device.isConnected()){
		console.log('im connected')
	}
	else {
		console.log('i have disconnected')
	
	}
	
	await new Promise(resolve => setTimeout(resolve, 1000)); //pauses the programme
	
	await device.disconnect()
  	destroy()
  	console.log('disconnected')
  	
	await new Promise(resolve => setTimeout(resolve, 1000)); //pauses the programme
	
  	
  	adapter.startDiscovery()
  	console.log('||      NODEJS     	||	Discovering')		//prints on command line					//
  										//								//
  	await adapter.waitDevice(deviceOfInterest)			//usese devices specified mac addess from top of programme	//
  	console.log('||      NODEJS     	||	got device', await device.getAddress())			//await device.getAddress())	//
  	await device.getName()				//gets the name of the device					//
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
  	await device.gatt()					//for recieving GATT services					//
  	await gattServer.services()					//for getting the serices					//
	console.log("||      NODEJS     	||	services are " + services)					//prints on command line//
} 
main()
  .then()
   .catch(console.error)
