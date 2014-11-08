#ifndef CDIRECTION_H
#define CDIRECTION_H

enum CDirectionType {
  CDIRECTION_TYPE_NONE  = 0,
  CDIRECTION_TYPE_UP    = (1<<0),
  CDIRECTION_TYPE_DOWN  = (1<<1),
  CDIRECTION_TYPE_LEFT  = (1<<2),
  CDIRECTION_TYPE_RIGHT = (1<<3)
};

#endif
