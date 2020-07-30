#include <stdint.h>
#include "stations.h"
#include <string.h>


StationPath* pathfindDirectional(StationPath *path, uint8_t from, uint8_t to, int8_t direction) {
  path->size = 0;
  
  if (direction != 1 && direction != -1) return path;
  if (direction == -1 && (from == STN_WATERFRONT || from == STN_LAFARGE)) return path;
  if (direction == 1 && (from == STN_VCC_CLARK || from == STN_KING_GEORGE)) return path;
  if (from == to) {
    path->path[path->size++] = from;
    return path;
  }

  uint8_t current = from;
  
  do {
    path->path[path->size++] = current;
    if (current == to) { // This station is the endpoint, finish the path.
      break;
    }
    if (current == STN_COLUMBIA) {
      // Split at Columbia.
      StationPath leftRight;
      memset(leftRight.path, 0, NUM_STATIONS);
      if (path->size < 2 || path->path[path->size - 2] != STN_SAPPERTON) pathfindDirectional(&leftRight, STN_SCOTT_ROAD, to, 1);
      if (leftRight.size == 0 && (path->size < 2 || path->path[path->size - 2] != STN_NEW_WESTMINSTER)) pathfindDirectional(&leftRight, STN_NEW_WESTMINSTER, to, -1);
      if (leftRight.size == 0 && path->size >= 2 && path->path[path->size - 2] == STN_SAPPERTON) {
        // Cannot backtrack onto itself. No path.
        path->size = 0;
        return path;
      }
      if (leftRight.size == 0) { // Didn't find it on the Expo Line.
        // Go onto the Sapperton-Braid branch. Manually doing these paths for simplicity.
        // Can't go onto the path if came from Scott Road.
        if (path->size >= 2 && path->path[path->size - 2] == STN_SCOTT_ROAD) {
          path->size = 0;
          return path;
        }
        if (to == STN_SAPPERTON) {
          path->path[path->size++] = STN_SAPPERTON;
          return path;
        } else if (to == STN_BRAID) {
          path->path[path->size++] = STN_SAPPERTON;
          path->path[path->size++] = STN_BRAID;
          return path;
        } else if (to == STN_LOUGHEED) {
          path->path[path->size++] = STN_SAPPERTON;
          path->path[path->size++] = STN_BRAID;
          path->path[path->size++] = STN_LOUGHEED;
          return path;
        } else {
          // From Lougheed, branch off west (or east).
          path->path[path->size++] = STN_SAPPERTON;
          path->path[path->size++] = STN_BRAID;
          path->path[path->size++] = STN_LOUGHEED;
          leftRight.size = 0;
          pathfindDirectional(&leftRight, STN_PRODUCTION, to, 1);
          if (leftRight.size == 0) { // No path in the end.
            path->size = 0;
          } else {
            for (int i = 0; i < leftRight.size; i++) {
              path->path[path->size++] = leftRight.path[i];
            }
          }
          return path;
        }
      } else {
        for (int i = 0; i < leftRight.size; i++) {
          path->path[path->size++] = leftRight.path[i];
        }
        return path;
      }
    } else if (current == STN_LOUGHEED) {
      // Split at Lougheed.
      StationPath leftRight;
      memset(leftRight.path, 0, NUM_STATIONS);
      if (path->size < 2 || path->path[path->size - 2] != STN_BRAID) pathfindDirectional(&leftRight, STN_BURQUITLAM, to, -1);
      if (leftRight.size == 0 && (path->size < 2 || path->path[path->size - 2] != STN_PRODUCTION)) pathfindDirectional(&leftRight, STN_PRODUCTION, to, 1);
      if (leftRight.size == 0 && path->size >= 2 && path->path[path->size - 2] == STN_BRAID) {
        // Cannot backtrack onto itself. No path.
        path->size = 0;
        return path;
      }
      if (leftRight.size == 0) { // Didn't find it on the Millennium Line.
        // Go onto the Braid-Sapperton branch. Manually doing these paths for simplicity.
        // Can't go onto the path if came from Burquitlam.
        if (path->size >= 2 && path->path[path->size - 2] == STN_BURQUITLAM) {
          path->size = 0;
          return path;
        }
        if (to == STN_BRAID) {
          path->path[path->size++] = STN_BRAID;
          return path;
        } else if (to == STN_SAPPERTON) {
          path->path[path->size++] = STN_BRAID;
          path->path[path->size++] = STN_SAPPERTON;
          return path;
        } else if (to == STN_COLUMBIA) {
          path->path[path->size++] = STN_BRAID;
          path->path[path->size++] = STN_SAPPERTON;
          path->path[path->size++] = STN_COLUMBIA;
          return path;
        } else {
          // From Columbia, branch off west (or east).
          path->path[path->size++] = STN_BRAID;
          path->path[path->size++] = STN_SAPPERTON;
          path->path[path->size++] = STN_COLUMBIA;
          leftRight.size = 0;
          pathfindDirectional(&leftRight, STN_NEW_WESTMINSTER, to, -1);
          if (leftRight.size == 0) { // No path in the end.
            path->size = 0;
          } else {
            for (int i = 0; i < leftRight.size; i++) {
              path->path[path->size++] = leftRight.path[i];
            }
          }
          return path;
        }
      } else {
        for (int i = 0; i < leftRight.size; i++) {
          path->path[path->size++] = leftRight.path[i];
        }
        return path;
      }
    } else if (current != from && (current == STN_WATERFRONT || current == STN_VCC_CLARK || current == STN_LAFARGE || current == STN_KING_GEORGE)) {
      // Terminus stations (and this is not the first stn). Didn't find a path.
      path->size = 0;
      return path;
    }

    if (current == STN_BRAID && direction == -1) {
      current = STN_LOUGHEED;
    } else if (current == STN_SAPPERTON && direction == 1) {
      current = STN_COLUMBIA;
    } else if (current == STN_BURQUITLAM && direction == 1) {
      current = STN_LOUGHEED;
    } else {
      current += direction;
    }
  } while (true);

  return path;
}

StationPath* pathfind(StationPath *path, uint8_t from, uint8_t to) {
  path->size = 0;
  memset(path->path, 0, NUM_STATIONS);
  if (from >= NUM_STATIONS || to >= NUM_STATIONS) return path;
  pathfindDirectional(path, from, to, 1);
  if (path->size > 0) return path;
  return pathfindDirectional(path, from, to, -1);
}
