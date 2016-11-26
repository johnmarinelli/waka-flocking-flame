#ifndef Particle_hpp
#define Particle_hpp

#include <stdio.h>
#include "cinder/gl/gl.h"

inline glm::vec3 limitVec3(const glm::vec3& a, float l) {
  return glm::normalize(a) * l;
}

class Particle {
  
public:
  Particle(const glm::vec3& loc);
  
  glm::vec3 cohesion(const std::vector<Particle>& neighbors) const;
  glm::vec3 steerTo(const Particle& other) const;
  glm::vec3 align(const std::vector<Particle>& neighbors) const;
  glm::vec3 separate(const std::vector<Particle>& neighbors) const;
  
  
  glm::vec3 flock(const std::vector<Particle>& neighbors);
  
  void update(const std::vector<Particle>& neighbors);
  void draw();
  
  glm::vec3 mLocation;
  glm::vec3 mVelocity;

  float mRadius;
};

#endif /* Particle_hpp */
