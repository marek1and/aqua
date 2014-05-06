package pl.marand.aqua;

import java.io.File;

import org.apache.commons.daemon.Daemon;
import org.apache.commons.daemon.DaemonContext;
import org.apache.commons.daemon.DaemonController;
import org.apache.commons.daemon.DaemonInitException;
import org.kohsuke.args4j.CmdLineException;
import org.kohsuke.args4j.CmdLineParser;
import org.kohsuke.args4j.Option;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import pl.marand.aquaconnect.connection.AquaConnection;
import pl.marand.aquaconnect.connection.AquaUSBConnection;

public class AquaUDPProxyServer implements Daemon {
	
	private static final Logger logger = LoggerFactory.getLogger(AquaUDPProxyServer.class);
	
	private AquaConnection connection;
	private ProxyServer server;
	private DaemonController controller;
	
	@Option(name="-dev", usage="USB device file name")
	private File dev;
	
	@Option(name="-port", usage="UDP server port")
	private int port = 4120;
	
	public File getDev() {
		return dev;
	}

	public int getPort() {
		return port;
	}

	private void parseArguments(String[] args) {
		CmdLineParser parser = new CmdLineParser(this);
        try {
        	parser.parseArgument(args);
        } catch (CmdLineException e) {
            controller.fail(e.getMessage());
        }
	}
	
	@Override
	public void init(DaemonContext context) throws DaemonInitException, Exception {
		controller = context.getController();
		parseArguments(context.getArguments());
		
		if(dev == null || !dev.exists()) {
			controller.fail(": Aqua UDP Proxy Server should be started with parameter -dev pointing to connected USB device!");
		} else {
			connection = new AquaUSBConnection(dev.getAbsolutePath());
			server = new ProxyServer(connection, port);
			
			Runtime.getRuntime().addShutdownHook(new ShutdownThread());
		}
	}

	@Override
	public void start() throws Exception {
		if(server != null) {
			logger.info("Starting UDP Proxy Server Daemon...");
			server.start();
			Thread.sleep(1000);
			if(server.isRunning()){
				logger.info("UDP Proxy Server Daemon started");
			} else {
				server.shutdown();
				controller.fail(": UDP Proxy Server Daemon starting failed");
			}
		}
	}

	@Override
	public void stop() throws Exception {
		if(server != null) {
			logger.info("Stopping UDP Proxy Server Daemon...");
			if(server.isRunning()){
				server.shutdown();
			}
			logger.info("UDP Proxy Server Daemon stopped");
		}
	}

	@Override
	public void destroy() {
		connection = null;
		server = null;
		controller = null;
	}
	
	private class ShutdownThread extends Thread {
		
		public void run(){
			if(server != null && server.isRunning()){
				server.shutdown();
				controller.shutdown();
			}
		}
		
	}

}
