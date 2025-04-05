// 基于CyberPi的标准库实现

func native(port, arg) {
    sfn port, arg;
    return arg;
}

func str(s) {
    return native("str", s);
}

func println(text) {
    return native("print", str(text));
}

func int(s) {
    return native("int", s);
}

func len(n) {
    return native("len", n);
}

func random() {
    return native("random", null);
}

func wait_ab() {
    return native("wait_ab", null);
}