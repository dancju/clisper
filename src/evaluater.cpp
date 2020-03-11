#ifndef EVALUATER_H__
#define EVALUATER_H__

bool eval(node &n) {
  if(n.is_atom()) {
    if(n.atom()==name2id["t"] || n.atom()==name2id["nil"])
      return true;
    printf("Undefined symbol: %s", id2name[n.atom()].c_str());
    return false;
  }
  if(n.hi()-n.lo()==0) {
    n.type = 0;
    n.atom() = name2id["nil"];
    return true;
  }
  if(n.lo()->is_list()) {
    printf("Wrong list.");
    return false;
  }
  node *t = n.lo();
  if(n.lo()->atom() == name2id["quote"]) {
    if(n.hi()-n.lo() != 2) {
      printf("Wrong number of arguments: %zu.\n", n.hi()-n.lo()-1);
      return false;
    }
    n = n.lo()[1];
    delete[] t;
    return true;
  } else if(n.lo()->atom() == name2id["atom"]) {
    if(n.hi()-n.lo() != 2) {
      printf("Wrong number of arguments: %zu.\n", n.hi()-n.lo()-1);
      return false;
    }
    if(!eval(n.lo()[1]))
      return false;
    n.type = 0;
    n.atom() = name2id[n.lo()[1].is_atom()||(n.lo()[1].is_list()&&n.lo()[1].hi()==n.lo()[1].lo())?"t":"nil"];
    delete[] t;
    return true;
  } else if(n.lo()->atom() == name2id["eq"]) {
    if(n.hi()-n.lo() != 3) {
      printf("Wrong number of arguments: %zu.\n", n.hi()-n.lo()-1);
      return false;
    }
    if(!eval(n.lo()[1]))
      return false;
    if(!eval(n.lo()[2]))
      return false;
    n.type = 0;
    if(n.lo()[1].is_atom() && n.lo()[2].is_atom()) {
      n.atom() = name2id[n.lo()[1].atom()==n.lo()[2].atom()?"t":"nil"];
    } else if(n.lo()[1].is_list() && n.lo()[2].is_list()) {
      n.atom() = name2id[n.lo()[1].hi()==n.lo()[1].lo()&&n.lo()[2].hi()==n.lo()[2].lo()?"t":"nil"];
    } else {
      n.atom() = name2id["nil"];
    }
    delete[] t;
    return true;
  } else if(n.lo()->atom() == name2id["car"]) {
    if(n.hi()-n.lo() != 2) {
      printf("Wrong number of arguments: %zu.\n", n.hi()-n.lo()-1);
      return false;
    }
    if(!eval(n.lo()[1]))
      return false;
    if(n.lo()[1].is_atom()) {
      printf("Wrong type argument: atom.\n");
      return false;
    }
    n = n.lo()[1].lo()[0];
    delete[] t;
    return true;
  } else if(n.lo()->atom() == name2id["cdr"]) {
    if(n.hi()-n.lo() != 2) {
      printf("Wrong number of arguments: %zu.\n", n.hi()-n.lo()-1);
      return false;
    }
    if(!eval(t[1]))
      return false;
    if(t[1].is_atom()) {
      printf("Wrong type argument: atom.\n");
      return false;
    }
    n.lo() = new node[t[1].hi()-t[1].lo()-1];
    n.hi() = n.lo()+(t[1].hi()-t[1].lo()-1);
    std::copy(t[1].lo()+1, t[1].hi(), n.lo());
    delete[] t;
    return true;
  } else if(n.lo()->atom() == name2id["cons"]) {
    if(n.hi()-n.lo() != 3) {
      printf("Wrong number of arguments: %zu.\n", n.hi()-n.lo()-1);
      return false;
    }
    if(!eval(t[1]))
      return false;
    if(!eval(t[2]))
      return false;
    if(t[1].is_list()) {
      printf("Wrong type argument: list.\n");
      return false;
    }
    if(t[2].is_atom()) {
      printf("Wrong type argument: atom.\n");
      return false;
    }
    n.lo() = new node[t[2].hi()-t[2].lo()+1];
    n.hi() = n.lo()+(t[2].hi()-t[2].lo()+1);
    n.lo()[0] = t[1];
    std::copy(t[2].lo(), t[2].hi(), n.lo()+1);
    delete[] t;
    return true;
  } else if(n.lo()->atom() == name2id["cond"]) {
    bool mark = false;
    for(auto i = n.lo()+1; i!=n.hi(); i++) {
      if(i->is_atom()) {
        printf("Wrong type argument: atom.\n");
        return false;
      }
      if(i->hi()-i->lo() != 2) {
        printf("Wrong number of arguments: %zu.\n", i->hi()-i->lo()-1);
        return false;
      }
      if(!eval(i->lo()[0]))
        return false;
      if(i->lo()[0].is_list())
        return false;
      if(id2name[i->lo()[0].atom()]=="t") {
        if(!eval(i->lo()[1]))
          return false;
        n = i->lo()[1];
        delete[] t;
        mark = true;
        break;
      }
    }
    if(!mark) {
      n.type = 0;
      n.atom() = name2id["nil"];
      delete[] t;
    }
    return true;
  } else if(n.lo()->atom() == name2id["lambda"]) {
    if(n.hi()-n.lo() != 3) {
      printf("Wrong number of arguments: %zu.\n", n.hi()-n.lo()-1);
      return false;
    }
    if(n.lo()[1].is_atom()) {
      puts("Wrong type argument: atom.");
      return false;
    }
    if(n.lo()[2].is_list()) {
      puts("Wrong type argument: list.");
      return false;
    }
    return true;
  } else {
    printf("Undefined operation: %s.\n", id2name[n.lo()[0].atom()].c_str());
    return false;
  }
}

#endif
