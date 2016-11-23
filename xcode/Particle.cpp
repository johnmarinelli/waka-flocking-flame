#include "Particle.hpp"
#include "cinder/Rand.h"

Particle::Particle(const glm::vec3& loc) :
  mLocation(loc),
  mDirection(ci::randVec3()),
  mVelocity(ci::randFloat(0.05f)),
  mRadius(ci::randFloat(0.5f))
{
}

glm::vec3 Particle::cohesion(const std::vector<Particle>& neighbors) const {
  glm::vec3 sum = std::accumulate(neighbors.begin(),
                                  neighbors.end(),
                                  glm::vec3{0},
                                  [] (const glm::vec3 a, const Particle& b) {
                                    return a + b.mLocation;
                                  });
  return sum / float(neighbors.size());
}

static const float MAX_STEER_SPEED = 1.f;
glm::vec3 Particle::steerTo(const Particle &other) const {
  auto otherLocation = other.mLocation;
  auto desired = otherLocation - mLocation;
  auto d = glm::length(desired); // glm::length is basically magnitude(vector)
  glm::vec3 steer{0};
  
  if (d > 0) {
    desired = glm::normalize(desired);
    
    desired *= MAX_STEER_SPEED;
    
    steer = desired - mVelocity;
    
    if (glm::length(steer) > MAX_STEER_SPEED) {
      steer = glm::normalize(steer) * MAX_STEER_SPEED;
    }
  }

  return steer;
}

static const float MAX_ALIGN_FORCE = 1.f;
glm::vec3 Particle::align(const std::vector<Particle>& neighbors) const {
  auto sum = std::accumulate(neighbors.begin(),
                                  neighbors.end(),
                                  glm::vec3{0},
                                  [] (const glm::vec3 a, const Particle& b) {
                                    return a + b.mDirection;
                                  });
  auto mean = sum / float(neighbors.size());
  
  if (glm::length(mean) > MAX_ALIGN_FORCE) {
    mean = glm::normalize(mean) * MAX_ALIGN_FORCE;
  };
  
  return mean;
}

static const float DESIRED_SEPARATION = 2.f;
glm::vec3 Particle::separate(const std::vector<Particle>& neighbors) const {
  auto sum = std::accumulate(neighbors.begin(),
                             neighbors.end(),
                             glm::vec3{0},
                             [this] (const glm::vec3 a, const Particle& b) {
                               glm::vec3 sep{0};
                               auto dis = glm::dot(mLocation, b.mLocation);
                               if (dis > 0 && dis < DESIRED_SEPARATION) {
                                 sep = glm::normalize(mLocation - b.mLocation) / dis;
                               }
                               return a + sep;
                             });
  
  return sum / float(neighbors.size());
}

void Particle::update() {
  mLocation += mDirection * mVelocity;
}

void Particle::draw() {
  cinder::gl::drawSphere(mLocation, mRadius);
}
