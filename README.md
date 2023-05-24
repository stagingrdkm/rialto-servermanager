# rialto-servermanager
Standalone Rialto Server Manager. It offers a websocket json-rpc api to be used by external application manager process and will spawn & manage the RialtoServer instances corresponding to each Rialto Application

## Running rialto-servermanager
By default the websocket server will listen on TCP port **10150**. Set environment var "RIALTO_SERVERMANAGER_WS_PORT" to change that default port.
You can set the base path where sockets are created by setting env var ""RIALTO_SERVERMANAGER_SOCKET_PATH". If not set, then "/tmp" will be used.

Environments variables to be passed to the server instances can be set by using environment variable "RIALTO_SERVER_ENV_VARS" (or "SESSION_SERVER_ENV_VARS").

> export RIALTO_SERVERMANAGER_SOCKET_PATH=/var/run/rialto
>
> export RIALTO_SERVERMANAGER_WS_PORT=10150
>
> export RIALTO_SERVER_ENV_VARS='OPEN_CDM_SERVER=/tmp/OCDM/ocdm;DBUS_SESSION_BUS_ADDRESS=unix:path=/var/run/dbus/system_bus_socket;nexus_ipc_dir=/var/run/nxserver;XDG_RUNTIME_DIR=/var/run/weston;XDG_DATA_DIRS=/usr/share/xkb/;HOME=/root;RIALTO_SINKS_RANK=0;GST_REGISTRY=/tmp/rialto-server-gstreamer-cache.bin;WAYLAND_DISPLAY=wayland-0;FORCE_SAP=TRUE;FORCE_SVP=TRUE;RIALTO_SINKS_RANK=0'
> 
> ./rialto-servermanager

## JSON websocket interface
JSON interface schema and docs: see ./docs/ subdir. 
### example: Register and receive events
> wscat -c ws://127.0.0.1:10150/jsonrpc  -x '{"jsonrpc":"2.0","id":1,"method":"com.libertyglobal.rdk.rialto.1.register", "params": { "event": "StateChangeEvent", "id": "events.1"  } }' -w 1000

### example: Start/Suspend/Resume/Stop session/socket
> wscat -c ws://127.0.0.1:10150/jsonrpc  -x '{"jsonrpc":"2.0","id":1,"method":"com.libertyglobal.rdk.rialto.1.start" }'
>
> wscat -c ws://127.0.0.1:10150/jsonrpc  -x '{"jsonrpc":"2.0","id":1,"method":"com.libertyglobal.rdk.rialto.1.suspend" }'
>
> wscat -c ws://127.0.0.1:10150/jsonrpc  -x '{"jsonrpc":"2.0","id":1,"method":"com.libertyglobal.rdk.rialto.1.resume" }'
>
> wscat -c ws://127.0.0.1:10150/jsonrpc  -x '{"jsonrpc":"2.0","id":1,"method":"com.libertyglobal.rdk.rialto.1.stop" }'
>
