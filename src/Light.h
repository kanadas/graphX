#ifndef __LIGHT_H_
#define __LIGHT_H_

#include "linalg.h"
#include "UniformBlock.h"

class Light : public UniformStruct
{
public:
    Light(vec4 ambient, vec4 direct, vec4 position)
        : ambient(ambient)
        , direct(direct)
        , position(position) {};

    void setAmbient(vec4 ambient) { this->ambient = ambient; }
    void setDirect(vec4 direct) { this->direct = direct; }
    void setPosition(vec4 position) { this->position = position; }
    vec4 getAmbient() { return ambient; }
    vec4 getDirect() { return direct; }
    vec4 getPosition() { return position; }

    virtual std::vector<std::string> getNames() const override;
    virtual void set() const override;
    virtual void setOffsets(std::vector<GLint>::iterator& offsets) override;
    void copyOffsets(const Light& other, GLint stride = 0);

private:
    vec4 ambient;
    vec4 direct;
    vec4 position;
    vec3 attenuation = vec3(1, 0, 0);
    GLint ambientOffset;
    GLint directOffset;
    GLint positionOffset;
    GLint attenuationOffset;
};

#endif // __LIGHT_H_
