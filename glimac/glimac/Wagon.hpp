namespace glimac {

    class Wagon {
    private:
        glm::vec3 m_Position;
        glm::vec3 m_FrontVector;
        glm::vec3 m_UpVector;

    public:
        Wagon() : m_Position{ glm::vec3(0, 0, 0) }, m_FrontVector{glm::vec3(0, 0, -1)}, m_UpVector{glm::vec3(0, 1, 0)}
        {}

        void move(glm::vec3 position, glm::vec3 frontVector, glm::vec3 upVector)
        {
            m_Position = position;
            m_FrontVector = frontVector;
            m_UpVector = upVector;
        }

        glm::mat4 getModelMatrix()
        {
            return glm::lookAt(m_Position, m_FrontVector, m_UpVector);
        }
    };
}