let connectButton;
let rotationCountElement;
let device;
let serviceUuid = '2c3955ff-3427-4915-a5de-26ab681c45e4';
let rotationCountCharacteristicUuid = '00000001-0000-0000-0000-000000000000';

function setup() {
  connectButton = select('#connectButton');
  rotationCountElement = select('#rotationCount');
  connectButton.mousePressed(connectToBluetooth);
}

async function connectToBluetooth() {
  try {
    console.log('Requesting Bluetooth device...');
    device = await navigator.bluetooth.requestDevice({
      filters: [{ services: [serviceUuid] }],
    });

    console.log('Connecting to GATT Server...');
    const server = await device.gatt.connect();

    console.log('Getting rotation service...');
    const service = await server.getPrimaryService(serviceUuid);

    console.log('Getting rotation count characteristic...');
    const rotationCountCharacteristic = await service.getCharacteristic(
      rotationCountCharacteristicUuid
    );

    device.addEventListener('gattserverdisconnected', onDisconnected);

    await rotationCountCharacteristic.startNotifications();
    rotationCountCharacteristic.addEventListener(
      'characteristicvaluechanged',
      (event) => {
        const value = event.target.value.getInt32(0, true);
        rotationCountElement.html(value);
      }
    );

    console.log('Connected to device and listening for rotation count updates');
  } catch (error) {
    console.error('Error:', error);
  }
}

function onDisconnected(event) {
  const device = event.target;
  console.log(`Device ${device.name} is disconnected.`);
}
