#include "../include/math/curve.h"
#include "../include/math/math.h"
#include "../include/math/transform.h"
#include "../include/entities/visuals/FragmentAnchor.h"

#include <glm.hpp>

float binomialCoefficient(int n, int k)
{
    // Using the multiplicative formula
    // en.wikipedia.org/wiki/Binomial_coefficient#Multiplicative_formula
    float c = 1.;
    for (int i = 1; i <= k; ++i)
        c *= float(n + 1 - i) / float(i);
    return c;
}

// en.wikipedia.org/wiki/B%C3%A9zier_curve
glm::vec2 explicitBezier(glm::vec2 control_points[4], float t)
{
    glm::vec2 sum = glm::vec2(0.0f);
    for (int i = 0; i <= 3; ++i)
    {
        sum += float(binomialCoefficient(3, i)) *
            float(pow(1. - t, float(3 - i)) * pow(t, float(i))) * control_points[i];
    }
    return sum;
}

// Exact BBox to a cubic bezier
// From IQ: shadertoy.com/view/XdVBWd
glm::vec4 bboxBezier(glm::vec2 p0, glm::vec2 p1, glm::vec2 p2, glm::vec2 p3)
{
    // extremes
    glm::vec2 mi = glm::min(p0, p3);
    glm::vec2 ma = glm::max(p0, p3);

    glm::vec2 k0 = -1.0f * p0 + 1.0f * p1;
    glm::vec2 k1 = 1.0f * p0 - 2.0f * p1 + 1.0f * p2;
    glm::vec2 k2 = -1.0f * p0 + 3.0f * p1 - 3.0f * p2 + 1.0f * p3;

    glm::vec2 h = k1 * k1 - k0 * k2;

    if (h.x > 0.0)
    {
        h.x = sqrt(h.x);
        //float t = (-k1.x - h.x)/k2.x;
        float t = k0.x / (-k1.x - h.x);
        if (t > 0.0f && t < 1.0f)
        {
            float s = 1.0f - t;
            float q = s * s * s * p0.x + 3.0f * s * s * t * p1.x + 3.0f * s * t * t * p2.x + t * t * t * p3.x;
            mi.x = glm::min(mi.x, q);
            ma.x = glm::max(ma.x, q);
        }
        //t = (-k1.x + h.x)/k2.x;
        t = k0.x / (-k1.x + h.x);
        if (t > 0.0f && t < 1.0f)
        {
            float s = 1.0f - t;
            float q = s * s * s * p0.x + 3.0f * s * s * t * p1.x + 3.0f * s * t * t * p2.x + t * t * t * p3.x;
            mi.x = glm::min(mi.x, q);
            ma.x = glm::max(ma.x, q);
        }
    }

    if (h.y > 0.0)
    {
        h.y = sqrt(h.y);
        //float t = (-k1.y - h.y)/k2.y;
        float t = k0.y / (-k1.y - h.y);
        if (t > 0.0f && t < 1.0f)
        {
            float s = 1.0 - t;
            float q = s * s * s * p0.y + 3.0f * s * s * t * p1.y + 3.0f * s * t * t * p2.y + t * t * t * p3.y;
            mi.y = glm::min(mi.y, q);
            ma.y = glm::max(ma.y, q);
        }
        //t = (-k1.y + h.y)/k2.y;
        t = k0.y / (-k1.y + h.y);
        if (t > 0.0f && t < 1.0f)
        {
            float s = 1.0f - t;
            float q = s * s * s * p0.y + 3.0f * s * s * t * p1.y + 3.0f * s * t * t * p2.y + t * t * t * p3.y;
            mi.y = glm::min(mi.y, q);
            ma.y = glm::max(ma.y, q);
        }
    }

    return glm::vec4(mi, ma);
}

bool computeTransformation(FragmentAnchor* start, FragmentAnchor* end, glm::vec3 &translation, glm::mat2 &rotation)
{
    // Set p0 to zero, to simplify computation
    glm::vec2 p1 = start->headHandle - start->pos;
    glm::vec2 p2 = end->tailHandle - start->pos;
    glm::vec2 p3 = end->pos - start->pos;

    // Polynomial coefficients of Bezier curve B(t)
    glm::vec2 a = 3.0f * (p1 - p2) + p3;
    glm::vec2 c = 3.0f * p1;
    glm::vec2 b = 3.0f * p2 - 2.0f * c;

    // Expected value, E[B] = mean of points along Bezier curve
    glm::vec2 mu = a / 4.0f + b / 3.0f + c / 2.0f;

    // Polynomial coefficients of Bx(t)By(t)
    float axy = a.x * a.y;
    float bxy = a.x * b.y + b.x * a.y;
    float cxy = a.x * c.y + b.x * b.y + c.x * a.y;
    float dxy = b.x * c.y + c.x * b.y;
    float exy = c.x * c.y;

    // Polynomial coefficients of Bx(t)Bx(t) and By(t)By(t)
    glm::vec2 aB = a * a;
    glm::vec2 bB = a * b * 2.0f;
    glm::vec2 cB = a * c * 2.0f + b * b;
    glm::vec2 dB = b * c * 2.0f;
    glm::vec2 eB = c * c;

    // Definite integrals of polynomials p(1) - p(0)
    float xy = axy / 7. + bxy / 6. + cxy / 5. + dxy / 4. + exy / 3.; // E[BxBy]

    float xx = aB.x / 7. + bB.x / 6. + cB.x / 5. + dB.x / 4. + eB.x / 3.; // E[BxBx]
    float yy = aB.y / 7. + bB.y / 6. + cB.y / 5. + dB.y / 4. + eB.y / 3.; // E[ByBy]

    // Covariance matrix
    glm::mat2 covariance = glm::mat2(0.0f);
    covariance[1][0] = covariance[0][1] = xy - mu.x * mu.y;
    covariance[0][0] = xx - mu.x * mu.x;
    covariance[1][1] = yy - mu.y * mu.y;

    // Characteristic polynomial of covariance matrix
    float eigen_a = 1.;
    float eigen_b = -(covariance[0][0] + covariance[1][1]);
    float eigen_c = covariance[0][0] * covariance[1][1] - covariance[1][0] * covariance[0][1];

    float discr = eigen_b * eigen_b - 4.0f * eigen_a * eigen_c;

    if (discr > 0.)
    {
        float sqrt_discr = sqrtf(discr);

        // Eigenvalues of covariance matrix
        float lambda0 = (-eigen_b - sqrt_discr) / (2.0f * eigen_a);
        float lambda1 = (-eigen_b + sqrt_discr) / (2.0f * eigen_a);

        // Eigenvectors of covariance matrix
        glm::vec2 eigenvector0 = glm::vec2(covariance[1][0], lambda0 - covariance[0][0]);
        glm::vec2 eigenvector1 = glm::vec2(covariance[1][0], lambda1 - covariance[0][0]);

        rotation[0] = glm::normalize(eigenvector0);
        rotation[1] = glm::normalize(eigenvector1);

        translation = glm::vec3(mu, 0.0f) + start->pos;
        return true;
    }

    return false;
}

bool getCubicBezierOBB(RectBounds &bounds, FragmentAnchor* start, FragmentAnchor* end, bool addPadding, float padding)
{
    glm::vec3 translation = glm::vec3(0.0f);
    glm::mat2 rotation = glm::mat2(0.0f);

    bool res = computeTransformation(start, end, translation, rotation);
    glm::vec2 translation2 = glm::vec2(translation.x, translation.y);

    if (!res)
        return false;

    // Transform the Bezier control points into the local space of the OBB
    glm::vec2 control_points_xfrm[4] = { glm::vec2(0.0f), glm::vec2(0.0f), glm::vec2(0.0f), glm::vec2(0.0f) };
    control_points_xfrm[0] = start->pos - translation;
    control_points_xfrm[0] = glm::transpose(rotation) * control_points_xfrm[0];
    control_points_xfrm[1] = start->headHandle - translation;
    control_points_xfrm[1] = glm::transpose(rotation) * control_points_xfrm[1];
    control_points_xfrm[2] = end->tailHandle - translation;
    control_points_xfrm[2] = glm::transpose(rotation) * control_points_xfrm[2];
    control_points_xfrm[3] = end->pos - translation;
    control_points_xfrm[3] = glm::transpose(rotation) * control_points_xfrm[3];

    // Get axis-aligned bounding box in local space
    glm::vec4 b2_xfrm = bboxBezier(control_points_xfrm[0], control_points_xfrm[1],
        control_points_xfrm[2], control_points_xfrm[3]);

    // Transform AABB back to worldspace
    bounds.p1 = glm::vec3(b2_xfrm.x, b2_xfrm.y, 0.0f);
    bounds.p1 = glm::vec3((rotation * bounds.p1) + translation2, 0.0f);
    bounds.p2 = glm::vec3(b2_xfrm.x, b2_xfrm.w, 0.0f);
    bounds.p2 = glm::vec3((rotation * bounds.p2) + translation2, 0.0f);
    bounds.p4 = glm::vec3(b2_xfrm.z, b2_xfrm.w, 0.0f);
    bounds.p4 = glm::vec3((rotation * bounds.p4) + translation2, 0.0f);
    bounds.p3 = glm::vec3(b2_xfrm.z, b2_xfrm.y, 0.0f);
    bounds.p3 = glm::vec3((rotation * bounds.p3) + translation2, 0.0f);

    // Add additional padding for proper rendering
    if (addPadding) {
        glm::vec3 leftDir = makeDir(bounds.p2, bounds.p1);
        glm::vec3 rightDir = makeDir(bounds.p3, bounds.p1);
        bounds.p1 += leftDir * padding + rightDir * padding;
        bounds.p2 += leftDir * -padding + rightDir * padding;
        bounds.p3 += leftDir * padding + rightDir * -padding;
        bounds.p4 += leftDir * -padding + rightDir * -padding;
    }

    return true;
}

bool getCubicBezierOBB(glm::vec3* p1, glm::vec3* p2, glm::vec3* p3, glm::vec3* p4,
    FragmentAnchor* start, FragmentAnchor* end, bool addPadding, float padding)
{
    glm::vec3 translation = glm::vec3(0.0f);
    glm::mat2 rotation = glm::mat2(0.0f);

    bool res = computeTransformation(start, end, translation, rotation);
    glm::vec2 translation2 = glm::vec2(translation.x, translation.y);

    if (!res)
        return false;

    // Transform the Bezier control points into the local space of the OBB
    glm::vec2 control_points_xfrm[4] = { glm::vec2(0.0f), glm::vec2(0.0f), glm::vec2(0.0f), glm::vec2(0.0f) };
    control_points_xfrm[0] = start->pos - translation;
    control_points_xfrm[0] = glm::transpose(rotation) * control_points_xfrm[0];
    control_points_xfrm[1] = start->headHandle - translation;
    control_points_xfrm[1] = glm::transpose(rotation) * control_points_xfrm[1];
    control_points_xfrm[2] = end->tailHandle - translation;
    control_points_xfrm[2] = glm::transpose(rotation) * control_points_xfrm[2];
    control_points_xfrm[3] = end->pos - translation;
    control_points_xfrm[3] = glm::transpose(rotation) * control_points_xfrm[3];

    // Get axis-aligned bounding box in local space
    glm::vec4 b2_xfrm = bboxBezier(control_points_xfrm[0], control_points_xfrm[1],
        control_points_xfrm[2], control_points_xfrm[3]);

    // Transform AABB back to worldspace
    *p1 = glm::vec3(b2_xfrm.x, b2_xfrm.y, 0.0f);
    *p1 = glm::vec3((rotation * *p1) + translation2, 0.0f);
    *p2 = glm::vec3(b2_xfrm.x, b2_xfrm.w, 0.0f);
    *p2 = glm::vec3((rotation * *p2) + translation2, 0.0f);
    *p3 = glm::vec3(b2_xfrm.z, b2_xfrm.w, 0.0f);
    *p3 = glm::vec3((rotation * *p3) + translation2, 0.0f);
    *p4 = glm::vec3(b2_xfrm.z, b2_xfrm.y, 0.0f);
    *p4 = glm::vec3((rotation * *p4) + translation2, 0.0f);

    // Add additional padding for proper rendering
    if (addPadding) {
        glm::vec3 leftDir = makeDir(*p2, *p1);
        glm::vec3 rightDir = makeDir(*p3, *p4);
        *p1 += leftDir * padding + rightDir * padding;
        *p2 += leftDir * -padding + rightDir * padding;
        *p3 += leftDir * padding + rightDir * -padding;
        *p4 += leftDir * -padding + rightDir * -padding;
    }

    return true;
}

void getCubicBezierPoints(FragmentAnchor* start, FragmentAnchor* end, 
    int resolution, float offset, std::vector<glm::vec3>& points, float& length, 
    float spacing, float startScale, float endScale)
{
    glm::vec2 control_points[4] = { glm::vec2(start->pos), glm::vec3(start->headHandle), glm::vec3(end->pos), glm::vec3(end->tailHandle) };
    getCubicBezierPoints(control_points, resolution, offset, points, length, spacing, startScale, endScale);
}

void getCubicBezierPoints(glm::vec2 control_points[4],
    int resolution, float offset, std::vector<glm::vec3>& points, float& length, 
    float spacing, float startScale, float endScale)
{
    std::vector<std::pair<float, glm::vec3>> rawBezier;
    for (int i = 0; i <= resolution; i++) {
        float t = (float)i / (float)resolution;
        rawBezier.push_back(std::pair<float, glm::vec3>(t, glm::vec3(explicitBezier(control_points, t), 0.0f)));
    }
    length = lengthPointVec(&rawBezier);
   
    int segCount = int(length / spacing);

    float usedLength = offset;
    std::pair<float, glm::vec3> point = rawBezier[0];
    std::pair<float, glm::vec3> next = rawBezier[1];
    size_t index = 0;
    float rawSegLength = 0.0f;
    float rawUsedLength = 0.0f;
    float storedLength = offset;

    while (usedLength < length && index < rawBezier.size() - 1) {
        point = rawBezier[index];
        next = rawBezier[index + 1];
        index++;
        rawSegLength = glm::length(next.second - point.second);
        rawUsedLength = 0.0f;
        float scale = lerpf(startScale, endScale, usedLength / length);
        if ((spacing * scale) - storedLength > rawSegLength) {
            storedLength += rawSegLength;
        }
        else {
            while (rawUsedLength < rawSegLength) {

                if (storedLength > 0.01) {
                    float remainder = ((spacing) - storedLength) / spacing;
                    if (remainder >= 1.0 || remainder <= 0) { 
                        std::cout << "REMAINER TOO LARGE :: " << remainder << std::endl; 
                    }
                    float scaleSpacingFactor = clampf(remainder * (spacing * scale), 0.2f, (float)INT_MAX);
                    float t = clampf(scaleSpacingFactor / rawSegLength, 0.0f, 1.0f);
                    points.push_back(glm::vec3(explicitBezier(control_points, lerpf(point.first, next.first, t)), 0.0f));
                    storedLength = 0;
                    rawUsedLength += scaleSpacingFactor;
                    usedLength += scaleSpacingFactor;
                }
                else {
                    float scaleSpacingFactor = clampf(spacing * scale, 0.2f, (float)INT_MAX);
                    float t = clampf((rawUsedLength + scaleSpacingFactor) / rawSegLength, 0.0f, 1.0f);
                    points.push_back(glm::vec3(explicitBezier(control_points, lerpf(point.first, next.first, t)), 0.0f));
                    rawUsedLength += scaleSpacingFactor;
                    usedLength += scaleSpacingFactor;
                }
                if (points.size() > 1 && points.back() == points[points.size() - 2]) {
                    std::cout << "STACKED POINTS :: " << points.size() - 1 << " , " << points.size() << std::endl;
                }
                scale = lerpf(startScale, endScale, usedLength / length);
                if (rawUsedLength + (spacing * scale) > rawSegLength) {
                    storedLength = abs(rawSegLength - rawUsedLength);
                    break;
                }
            }
        }
    }

    return;
}