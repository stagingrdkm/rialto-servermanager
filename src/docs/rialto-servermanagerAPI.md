<!-- Generated automatically, DO NOT EDIT! -->
<a name="head.rialto-servermanager_API"></a>
# rialto-servermanager API

**Version: 0.0.1**

### Table of Contents

- [Description](#head.Description)
- [Methods](#head.Methods)
- [Notifications](#head.Notifications)

<a name="head.Description"></a>
# Description

rialto-servermanager JSON RPC 2.0 interface.

<a name="head.Methods"></a>
# Methods

The following methods are provided by the com.libertyglobal.rdk.rialto interface:

| Method | Description |
| :-------- | :-------- |
| [activate](#method.activate) | Activates an app session |
| [stop](#method.stop) | Stops an app session and terminate the server |
| [deactivate](#method.deactivate) | Suspends an app session (Moves it to 'inactive' state) |
| [getState](#method.getState) | Gets session state |
| [register](#method.register) | Registers event listener |
| [unregister](#method.unregister) | Registers event listener |
| [getListeners](#method.getListeners) | Returns information about registered listeners |


<a name="method.activate"></a>
## *activate <sup>method</sup>*

Activates an app session.

Spawns a server instance if app session was in 'notrunning' state.
If App session server state changes, then StateChangeEvent 'active' will be sent.
If App session server is already in 'active' state, then method call will succeed but no StateChangeEvent will be sent.

Also see: [StateChangeEvent](#event.StateChangeEvent)

### Parameters

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| params | object |  |
| params.id | string | Id of session/app. Will also be used as the socket name. Cannot be empty. Accepted characters are [A-Za-z0-9._-] |

### Result

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| result | object |  |
| result.socket | string | Path of the socket created |

### Errors

| Code | Message | Description |
| :-------- | :-------- | :-------- |
| 2 | ```GERROR_GENERAL: general error``` |  |
| 1 | ```BAD_ARGUMENTS: Invalid method parameters (invalid name and/or type) recognised: Wrong parameters``` |  |

### Example

#### Request

```json
{
    "jsonrpc": "2.0",
    "id": 1234567890,
    "method": "com.libertyglobal.rdk.rialto.1.activate",
    "params": {
        "id": "app"
    }
}
```

#### Response

```json
{
    "jsonrpc": "2.0",
    "id": 1234567890,
    "result": {
        "socket": "/tmp/app"
    }
}
```

<a name="method.stop"></a>
## *stop <sup>method</sup>*

Stops an app session and terminate the server.

App session server is moved to 'notrunning' state.
If App session server state changes, then StateChangeEvent 'notrunning' will be sent.
If App session server is already in 'notrunning' state, then method call will succeed but no StateChangeEvent will be sent.

Also see: [StateChangeEvent](#event.StateChangeEvent)

### Parameters

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| params | object |  |
| params.id | string | Id of session/app. Will also be used as the socket name. Cannot be empty. Accepted characters are [A-Za-z0-9._-] |

### Result

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| result | null | Always null |

### Errors

| Code | Message | Description |
| :-------- | :-------- | :-------- |
| 2 | ```GERROR_GENERAL: general error``` |  |
| 1 | ```BAD_ARGUMENTS: Invalid method parameters (invalid name and/or type) recognised: Wrong parameters``` |  |

### Example

#### Request

```json
{
    "jsonrpc": "2.0",
    "id": 1234567890,
    "method": "com.libertyglobal.rdk.rialto.1.stop",
    "params": {
        "id": "app"
    }
}
```

#### Response

```json
{
    "jsonrpc": "2.0",
    "id": 1234567890,
    "result": null
}
```

<a name="method.deactivate"></a>
## *deactivate <sup>method</sup>*

Suspends an app session (Moves it to 'inactive' state).

Causes server to terminate A/V playback sessions.
If App session server state is changed, then StateChangeEvent 'inactive' will be sent.
If App session server is already in 'inactive' state, then method call will succeed but no StateChangeEvent will be sent.

Also see: [StateChangeEvent](#event.StateChangeEvent)

### Parameters

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| params | object |  |
| params.id | string | Id of session/app. Will also be used as the socket name. Cannot be empty. Accepted characters are [A-Za-z0-9._-] |

### Result

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| result | object |  |
| result.socket | string | Path of the socket created |

### Errors

| Code | Message | Description |
| :-------- | :-------- | :-------- |
| 2 | ```GERROR_GENERAL: general error``` |  |
| 1 | ```BAD_ARGUMENTS: Invalid method parameters (invalid name and/or type) recognised: Wrong parameters``` |  |

### Example

#### Request

```json
{
    "jsonrpc": "2.0",
    "id": 1234567890,
    "method": "com.libertyglobal.rdk.rialto.1.deactivate",
    "params": {
        "id": "app"
    }
}
```

#### Response

```json
{
    "jsonrpc": "2.0",
    "id": 1234567890,
    "result": {
        "socket": "/tmp/app"
    }
}
```

<a name="method.getState"></a>
## *getState <sup>method</sup>*

Gets session state.

### Parameters

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| params | object |  |
| params.id | string | Id of session/app. Will also be used as the socket name. Cannot be empty. Accepted characters are [A-Za-z0-9._-] |

### Result

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| result | object |  |
| result.state | string | State of the session: uninitialized, inactive, active, notrunning, error |

### Errors

| Code | Message | Description |
| :-------- | :-------- | :-------- |
| 1 | ```BAD_ARGUMENTS: Invalid method parameters (invalid name and/or type) recognised: Wrong parameters``` |  |

### Example

#### Request

```json
{
    "jsonrpc": "2.0",
    "id": 1234567890,
    "method": "com.libertyglobal.rdk.rialto.1.getState",
    "params": {
        "id": "app"
    }
}
```

#### Response

```json
{
    "jsonrpc": "2.0",
    "id": 1234567890,
    "result": {
        "state": "active"
    }
}
```

<a name="method.register"></a>
## *register <sup>method</sup>*

Registers event listener.

### Parameters

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| params | object |  |
| params.event | string | Event type (must be one of the following: *StateChangeEvent*) |
| params.id | string | Id of the listener |

### Result

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| result | integer | Value 0 on success |

### Errors

| Code | Message | Description |
| :-------- | :-------- | :-------- |
| -1 | ```Registration info not found/Only one registration per event type per each connection``` |  |

### Example

#### Request

```json
{
    "jsonrpc": "2.0",
    "id": 1234567890,
    "method": "com.libertyglobal.rdk.rialto.1.register",
    "params": {
        "event": "StateChangeEvent",
        "id": "events.1"
    }
}
```

#### Response

```json
{
    "jsonrpc": "2.0",
    "id": 1234567890,
    "result": 0
}
```

<a name="method.unregister"></a>
## *unregister <sup>method</sup>*

Registers event listener.

### Parameters

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| params | object |  |
| params?.event | string | <sup>*(optional)*</sup> Event type (must be one of the following: *StateChangeEvent*) |
| params.id | string | Id of the listener |

### Result

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| result | integer | Value 0 on success |

### Errors

| Code | Message | Description |
| :-------- | :-------- | :-------- |
| 1 | ```BAD_ARGUMENTS: Invalid method parameters (invalid name and/or type) recognised: Wrong parameters``` |  |
| -1 | ```Registration info not found/Only one registration per event type per each connection``` |  |

### Example

#### Request

```json
{
    "jsonrpc": "2.0",
    "id": 1234567890,
    "method": "com.libertyglobal.rdk.rialto.1.unregister",
    "params": {
        "event": "StateChangeEvent",
        "id": "events.1"
    }
}
```

#### Response

```json
{
    "jsonrpc": "2.0",
    "id": 1234567890,
    "result": 0
}
```

<a name="method.getListeners"></a>
## *getListeners <sup>method</sup>*

Returns information about registered listeners.

### Parameters

This method takes no parameters.

### Result

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| result | object |  |
| result.listeners | array |  |
| result.listeners[#] | object | Listener Info |
| result.listeners[#]?.event | string | <sup>*(optional)*</sup> Event type (must be one of the following: *StateChangeEvent*) |

### Errors

| Code | Message | Description |
| :-------- | :-------- | :-------- |

### Example

#### Request

```json
{
    "jsonrpc": "2.0",
    "id": 1234567890,
    "method": "com.libertyglobal.rdk.rialto.1.getListeners"
}
```

#### Response

```json
{
    "jsonrpc": "2.0",
    "id": 1234567890,
    "result": {
        "listeners": [
            {
                "event": "StateChangeEvent"
            }
        ]
    }
}
```

<a name="head.Notifications"></a>
# Notifications

Notifications are autonomous events, triggered by the internals of the implementation, and broadcasted via JSON-RPC to all registered observers. 

The following events are provided by the com.libertyglobal.rdk.rialto interface:

| Event | Description |
| :-------- | :-------- |
| [StateChangeEvent](#event.StateChangeEvent) | Rialto-ServerManager state changed event |


<a name="event.StateChangeEvent"></a>
## *StateChangeEvent <sup>event</sup>*

Rialto-ServerManager state changed event.

### Description

Notification of changed state of rialto-servermanager

### Parameters

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| params | object |  |
| params.id | string | Id of session/app. Will also be used as the socket name. Cannot be empty. Accepted characters are [A-Za-z0-9._-] |
| params.state | string | State of the session: uninitialized, inactive, active, notrunning, error |

### Example

```json
{
    "jsonrpc": "2.0",
    "method": "client.events.1.StateChangeEvent",
    "params": {
        "id": "app",
        "state": "active"
    }
}
```

