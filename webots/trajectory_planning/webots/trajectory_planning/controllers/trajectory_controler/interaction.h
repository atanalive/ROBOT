#ifndef INTERACTION
#define INTERACTION

void interaction_init(void);
int choose_mode(void);
void mouse_get_position(void);
int choose_trajectory_type(void);

extern WbMouseState mouse;

#endif
