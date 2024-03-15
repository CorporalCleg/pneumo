from press_app_qt.app_x.tests import events

def register_user(username: str, password: str) -> None:
    print(f'save username {username} with paswd {password}')
    events.dispatch('user_registred', username)

def send_email(username: str):
    print(f'Send emain from {username}')

def notify_friends(username: str):
    print(f"All {username}'s friends notified")

events.register_handler('user_registred', send_email)
events.register_handler('user_registred', notify_friends)

register_user('Max', '1234')