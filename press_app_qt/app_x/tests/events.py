
event_handler = {}

def register_handler(event: str, func: callable):
    functions = event_handler.get(event)

    if functions is None:
        event_handler[event] = [func]
    else:
        event_handler[event].append(func)

def dispatch(event: str, data):
    functions = event_handler.get(event)

    if functions is None:
        raise ValueError(f'Unknown event: {event}')
    else:
        for func in functions:
            func(data)