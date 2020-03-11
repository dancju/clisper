#ifndef PARSER_H__
#define PARSER_H__

std::vector<std::string> id2name;
std::map<std::string, size_t> name2id;

size_t add_symbol(std::string s) {
  assert(name2id.find(s)==name2id.end());
  id2name.push_back(s);
  return name2id[s] = id2name.size()-1;
}

// data structure for generalized list
// 廣義表之數據結構
struct node {
  bool type;
  union {
    size_t atom;
    struct {
      node *lo, *hi;
    } list;
  } val;
  node() {}
  node(const std::string &_atom) {
    type = 0;
    if(name2id.find(_atom)==name2id.end()) {
      val.atom = add_symbol(_atom);
    } else {
      val.atom = name2id[_atom];
    }
  }
  node(node *_lo, node *_hi) {
    type = 1;
    val.list.lo = _lo;
    val.list.hi = _hi;
  }
  bool is_atom() const { return type==0; }
  bool is_list() const { return type==1; }
  size_t& atom() { return val.atom; }
  node*& lo() { return val.list.lo; }
  node*& hi() { return val.list.hi; }
};

std::list<node> nodes;

void _print(node& n, int depth = 0) {
  for(int i = 0; i<depth; i++)
    putchar('\t');
  if(n.is_atom()) {
    printf("%s\n", id2name[n.atom()].c_str());
  } else {
    puts("()");
    std::for_each(n.lo(), n.hi(), [&](node &n) {
      _print(n, depth+1);
    });
  }
}

void print(node& n) {
  if(n.is_atom()) {
    printf("%s ", id2name[n.atom()].c_str());
  } else {
    putchar('(');
    std::for_each(n.lo(), n.hi(), [&](node &n) {
      print(n);
    });
    putchar('\b');
    putchar(')');
  }
}

bool parse(std::list<node>::iterator lo) {
  assert(lo->is_atom());
  if(id2name[lo->atom()]=="'") {
    lo->type = 1;
    lo->lo() = new node[2];
    lo->hi() = lo->lo()+2;
    lo->lo()[0] = node("quote");
    parse(next(lo));
    lo->lo()[1] = *next(lo);
    nodes.erase(next(lo));
    return true;
  } else if(lo->atom()==name2id["("]) {
    lo->type = 1;
    std::vector<node> _l;
    for(std::list<node>::iterator i = next(lo); i!=nodes.end(); i++) {
      if(i->is_atom() && i->atom()==name2id[")"]) {
        lo->lo() = new node[_l.size()];
        lo->hi() = lo->lo()+_l.size();
        for(int j = 0; j<_l.size(); j++)
          lo->lo()[j] = _l[j];
        nodes.erase(i);
        return true;
      } else if(i->is_atom() && i->atom()==name2id["'"]) {
        parse(i);
        _l.push_back(*i);
        nodes.erase(i);
      } else if(i->is_atom() && i->atom()==name2id["("]) {
        parse(i);
        _l.push_back(*i);
        nodes.erase(i);
      } else {
        _l.push_back(*i);
        nodes.erase(i);
      }
    }
    return false;
  } else if(lo->atom()==name2id[")"]) {
    return false;
  } else
    return true;
}

#endif
