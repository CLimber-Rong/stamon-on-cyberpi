import cyberpi;

def en_list = {
    "textbook",
    "aloud",
    "secret",
    "repeat",
    "physics",
    "connect",
    "review",
    "note",
    "brain",
    "warn",
    "folk",
    "lay",
    "garden"
};

def cn_list = {
    "n.教科书",
    "adv.大声地",
    "adj.秘密的",
    "v.重复",
    "n.物理",
    "v.连接",
    "v.复习",
    "n.笔记",
    "n.大脑",
    "v.警告",
    "adj.民间的",
    "v.安放",
    "n.花园"
};

def l = len(en_list) - 1;
def index = random() % l;

println(en_list[index]);
wait_ab();
println(cn_list[index]);