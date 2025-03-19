from typing import Union, Optional

def str2int(s: str) -> Optional[int]:
    digits = ['0', '1', '2', '3', '4', '5', '6', '7', '8', '9']
    numbers = ['zero', 'uno', 'due', 'tre', 'quattro', 'cinque', 'sei', 'sette', 'otto', 'nove']
    if s in digits:
        return int(s)
    if s in numbers:
        return numbers.index(s)
    return None