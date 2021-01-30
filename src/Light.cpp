#include "Light.h"
#include "utils/Log.h"
#include <algorithm>

std::vector<std::string> Light::getNames() const
{
     return { "ambient", "direct", "position", "attenuation" };
}

void Light::set() const
{
    /*
    DEBUG("Ambient (offset {}): {} {} {} {}", ambientOffset, ambient.arr[0], ambient.arr[1], ambient.arr[2], ambient.arr[3]);
    DEBUG("Direct (offset {}): {} {} {} {}", directOffset, direct.arr[0], direct.arr[1], direct.arr[2], direct.arr[3]);
    DEBUG("Position (offset {}): {} {} {} {}", positionOffset, position.arr[0], position.arr[1], position.arr[2], position.arr[3]);
    DEBUG("Attenuation (offset {}): {} {} {}", attenuationOffset, attenuation.arr[0], attenuation.arr[1], attenuation.arr[2]);
    */
    setData(ambient.arr, 4 * sizeof(GLfloat), ambientOffset);
    setData(direct.arr, 4 * sizeof(GLfloat), directOffset);
    setData(position.arr, 4 * sizeof(GLfloat), positionOffset);
    setData(attenuation.arr, 3 * sizeof(GLfloat), attenuationOffset);
}

void Light::setOffsets(std::vector<GLint>::iterator& offsets)
{
    ambientOffset = *offsets;
    offsets++;
    directOffset = *offsets;
    offsets++;
    positionOffset = *offsets;
    offsets++;
    attenuationOffset = *offsets;
}

void Light::copyOffsets(const Light& other, GLint stride /*= 0*/)
{
    ambientOffset = other.ambientOffset + stride;
    directOffset = other.directOffset + stride;
    positionOffset = other.positionOffset + stride;
    attenuationOffset = other.attenuationOffset + stride;
}
