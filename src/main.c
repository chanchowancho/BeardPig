#include <pebble.h>

static Window *s_main_window;
static TextLayer *s_time_layer;
static BitmapLayer *s_background_layer;
static BitmapLayer *s_day_layer;
static GBitmap *s_background_bitmap;
static GBitmap *s_background_bitmap_two;
static GBitmap *s_background_bitmap_up;
static GBitmap *s_background_bitmap_down;
static GBitmap *s_background_bitmap_both_up;
static GBitmap *s_background_bitmap_both_down;
static GBitmap *s_day;

static void main_window_load(Window *window) {
  // Create GBitmap, then set to created BitmapLayer
  s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_BACKGROUND);
  s_background_bitmap_two = gbitmap_create_with_resource(RESOURCE_ID_BACKGROUND2);
  s_background_bitmap_up = gbitmap_create_with_resource(RESOURCE_ID_BACKGROUND_UP);
  s_background_bitmap_down = gbitmap_create_with_resource(RESOURCE_ID_BACKGROUND_DOWN);
  s_background_bitmap_both_up = gbitmap_create_with_resource(RESOURCE_ID_BACKGROUND_BOTH_UP);
  s_background_bitmap_both_down = gbitmap_create_with_resource(RESOURCE_ID_BACKGROUND_BOTH_DOWN);
  
  s_day = gbitmap_create_with_resource(RESOURCE_ID_MONDAY);

  s_background_layer = bitmap_layer_create(GRect(0, 0, 144, 168));
  bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap);
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_background_layer));
  
  s_day_layer = bitmap_layer_create(GRect(0, 140, 144, 28));
  bitmap_layer_set_bitmap(s_day_layer, s_day);
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_day_layer));
 
  
  
  
  
  // Create time TextLayer
  s_time_layer = text_layer_create(GRect(0, 8, 144, 50));
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorBlack);
  text_layer_set_text(s_time_layer, "00  00");

  // Improve the layout to be more like a watchface
  text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);

  // Add it as a child layer to the Window's root layer
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
  
  
}

static void update_date(int input){
  switch(input){
    case 0:
    gbitmap_destroy(s_day);
    s_day = gbitmap_create_with_resource(RESOURCE_ID_SUNDAY);
    bitmap_layer_set_bitmap(s_day_layer,s_day);
    break;
    case 1:
    gbitmap_destroy(s_day);
    s_day = gbitmap_create_with_resource(RESOURCE_ID_MONDAY);
    bitmap_layer_set_bitmap(s_day_layer,s_day);
    break;
    case 2:
    gbitmap_destroy(s_day);
    s_day = gbitmap_create_with_resource(RESOURCE_ID_TUESDAY);
    bitmap_layer_set_bitmap(s_day_layer,s_day);
    break;
    case 3:
    gbitmap_destroy(s_day);
    s_day = gbitmap_create_with_resource(RESOURCE_ID_WEDNESDAY);
    bitmap_layer_set_bitmap(s_day_layer,s_day);
    break;
    case 4:
    gbitmap_destroy(s_day);
    s_day = gbitmap_create_with_resource(RESOURCE_ID_THURSDAY);
    bitmap_layer_set_bitmap(s_day_layer,s_day);
    break;
    case 5:
    gbitmap_destroy(s_day);
    s_day = gbitmap_create_with_resource(RESOURCE_ID_FRIDAY);
    bitmap_layer_set_bitmap(s_day_layer,s_day);
    break;
    case 6:
    gbitmap_destroy(s_day);
    s_day = gbitmap_create_with_resource(RESOURCE_ID_SATURDAY);
    bitmap_layer_set_bitmap(s_day_layer,s_day);
    break;
  }
}

static void update_background(int counter,int minutesCounter){
  if (counter % 2 == 0){
    if ((counter == 56 && minutesCounter == 59) || (counter == 58 && minutesCounter == 59)){
      bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap_both_up);
    } else if (counter == 56 || counter == 58) {
      bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap_up);
    } else {
      bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap_two);
    } 
  } else {
    if (counter == 57 && minutesCounter == 59){
      bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap_both_down);
    } else if (counter == 57 ) {
      bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap_down);
    } else {
      bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap);
    }
  }
  
}

static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);
  
  static char secondString[] = "11";
  strftime(secondString, sizeof("11"), "%S", tick_time);
  int counter = atoi(secondString);
  
  static char minutesString[] = "22";
  strftime(minutesString, sizeof("22"), "%M", tick_time);
  int minuteCounter = atoi(minutesString);
  
  update_background(counter,minuteCounter);
  
  static char daystring[] = "1";
  strftime(daystring, sizeof("1"), "%w", tick_time);
  int weekdayNum = atoi(daystring);
  update_date(weekdayNum);


  
  // Create a long-lived buffer
  static char buffer[] = "00  00";

  // Write the current hours and into the buffer
  if(clock_is_24h_style() == true) {
    // Use 24 hour format
    strftime(buffer, sizeof("00  00"), "%H  %M", tick_time);
  } else {
    // Use 12 hour format
    strftime(buffer, sizeof("00  00"), "%I  %M", tick_time);
  }

  // Display this time on the TextLayer
  text_layer_set_text(s_time_layer, buffer);
}


static void main_window_unload(Window *window) {
  // Destroy GBitmap
  gbitmap_destroy(s_background_bitmap);
  gbitmap_destroy(s_background_bitmap_two);
  gbitmap_destroy(s_background_bitmap_up);
  gbitmap_destroy(s_background_bitmap_down);
  gbitmap_destroy(s_background_bitmap_both_up);
  gbitmap_destroy(s_background_bitmap_both_down);
  gbitmap_destroy(s_day);
  bitmap_layer_destroy(s_background_layer);
  bitmap_layer_destroy(s_day_layer);
  text_layer_destroy(s_time_layer);
  //window_destroy(s_main_window);
  
  
  
 
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

  
static void init() {
   // Create main Window element and assign to pointer
  s_main_window = window_create();
  window_set_fullscreen(s_main_window, true);

  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  // Show the Window on the watch, with animated=true
  
  window_stack_push(s_main_window, true);
  update_time();

  // Register with TickTimerService
  tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
}


static void deinit() {
  window_destroy(s_main_window);
}


int main(void) {
  init();
  app_event_loop();
  deinit();
}