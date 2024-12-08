const dgram = require('dgram');
const net = require('net');

var vc = "";

// Crear un socket UDP
const server = dgram.createSocket('udp4');

// Evento que se dispara cuando el servidor está listo
server.on('listening', () => {
    const address = server.address();
    console.log(`Servidor UDP escuchando en ${address.address}:${address.port}`);
});


// Evento que se dispara al recibir un mensaje
server.on('message', (msg, rinfo) => {

    //console.log(msg.toString());

});


// Manejo de errores
server.on('error', (err) => {
    console.error(`Error en el servidor: ${err.stack}`);
    server.close();
});

// Iniciar el servidor en el puerto 41234
const PORT = 50;
server.bind(PORT);


/*//crear servidor tcp en el puert 12345 con nodejs


//inclur llibreri para servser udp


*/

const servidor = net.createServer((sock) => {
    
    console.log('Conexión establecida');


    server.on('message', (msg, rinfo) => {

        console.log(msg.toString());
        sock.write(msg.toString());
    });
    
    


  sock.on('data', (datos) => {
    console.log(`Recibido: ${datos}`);
    //sock.write('Hola cliente!');

  });

  sock.on('end', () => {
    console.log('Conexión finalizada');
  });


});

servidor.listen(12345,() => {
  console.log('Servidor escuchando en puerto 12345');
});

