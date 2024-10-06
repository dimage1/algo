use std::env;
use std::net::UdpSocket;

fn main() -> std::io::Result<()> {
    {
        let args: Vec<String> = env::args().collect();

        let mut port = "34567";
        if args.len() > 1 {
            port = &args[1];
        }
        
        let mut addr:  String = "127.0.0.1:".to_owned();
        addr.push_str(port);
        let socket = UdpSocket::bind(addr)?;

        // Receives a single datagram message on the socket. If `buf` is too small to hold
        // the message, it will be cut off.
        let mut buf = [0; 10];

        loop {
            let (amt, src) = socket.recv_from(&mut buf)?;

            // Redeclare `buf` as slice of the received data and send reverse data back to origin.
            let buf = &mut buf[..amt];
            buf.reverse();
            socket.send_to(buf, &src)?;
        }
    } 
    // the socket is closed here
}