#ifndef TOKENIZER_H__
#define TOKENIZER_H__

std::vector<std::string> tokens;

bool tokenize(std::istream& in) {
  in >> std::noskipws;
  uint8_t state = 0;
  std::string buff;
  for(char c; !in.eof();) {
    in>>c;
    switch(state) {
    case 0:
      if(c==' ' || c=='\t' || c=='\n') {
      } else if(c=='(') {
        tokens.push_back("(");
      } else if(c==')') {
        tokens.push_back(")");
      } else if(c=='\'') {
        tokens.push_back("'");
      } else if(c==';') {
        state = 2;
      } else {
        buff = c;
        state = 1;
      }
      break;
    case 1:
      if(c==' ' || c=='\t' || c=='\n') {
        tokens.push_back(buff);
        buff = "";
        state = 0;
      } else if(c==';') {
        tokens.push_back(buff);
        buff = "";
        state = 2;
      }else if(c=='(') {
        tokens.push_back(buff);
        buff = "";
        tokens.push_back("(");
        state = 0;
      } else if(c==')') {
        tokens.push_back(buff);
        buff = "";
        tokens.push_back(")");
        state = 0;
      } else {
        buff.push_back(c);
      }
      break;
    case 2:
      if(c=='\n')
        state = 0;
      break;
    }
  }
  if(state) {
    return false;
  } else
    return true;
}

#endif
