#ifndef Particle_hpp
#define Particle_hpp

#include <stdio.h>
#include "cinder/gl/gl.h"

class Particle {
  
public:
  Particle(const glm::vec3& loc);
  
  glm::vec3 cohesion(const std::vector<Particle>& neighbors) const;
  glm::vec3 steerTo(const Particle& other) const;
  glm::vec3 align(const std::vector<Particle>& neighbors) const;
  glm::vec3 separate(const std::vector<Particle>& neighbors) const;
  
  void update();
  void draw();
  
  glm::vec3 mLocation;
  glm::vec3 mDirection;
  float mVelocity;
  float mRadius;
};

#endif /* Particle_hpp */
